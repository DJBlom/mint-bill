/*****************************************************************************
 * @file    invoice_pdf.cpp
 *
 * @brief
 *   Implementation of the Mint-Bill invoice PDF generator.
 *
 * @details
 *   This source file implements `feature::invoice_pdf`, which converts a
 *   `data::pdf_invoice` object into a fully formatted PDF invoice using
 *   Cairo / cairomm.
 *
 *   High-level flow (`generate()`):
 *     1. Accept a `std::any` that is expected to contain `data::pdf_invoice`.
 *     2. Validate the aggregate (`data::pdf_invoice::is_valid()`).
 *     3. Create a `Cairo::PdfSurface` backed by an in-memory stream and a
 *        `Cairo::Context` bound to that surface.
 *     4. Configure a monospace font and fixed page dimensions.
 *     5. Render, in sequence:
 *          - Document header (e.g., "Invoice").
 *          - Business / client information block.
 *          - Invoice metadata (ID, date, order number, job card).
 *          - Labor section (table, rows, and subtotal).
 *          - Material section (table, rows, and subtotal).
 *          - Grand total summary.
 *          - Payment method details and client message.
 *     6. Finalize the page with `show_page()` and `finish()` and return the
 *        accumulated PDF bytes as a `std::string`.
 *
 *   Layout helpers:
 *     - `add_header`, `add_information`, `add_invoice`,
 *       `add_labor`, `add_material`, `add_grand_total`,
 *       `add_payment_method`:
 *         * Compose domain-level sections using the underlying drawing
 *           primitives (`write_to_pdf*`, `draw_line`, alignment helpers).
 *
 *     - `add_items` / `add_item_description`:
 *         * Iterate over `data::column` vectors to render tabular rows.
 *         * Use `utility::boundary_slicer` to wrap long descriptions into
 *           multiple lines while preserving column alignment.
 *
 *     - `write_to_pdf`, `write_to_pdf_in_center`,
 *       `write_to_pdf_from_right`, `write_to_pdf_from_right_information`:
 *         * Configure font size, compute `Cairo::TextExtents`, update the
 *           current cursor position, and render the given string.
 *
 *   Page and cursor management:
 *     - `add_new_line`:
 *         * Advances the vertical cursor and triggers a new page if the
 *           next line would exceed the bottom margin.
 *
 *     - `add_new_section`:
 *         * Adds vertical spacing between logical sections.
 *
 *     - `align_to_left_border`, `align_to_right_border`,
 *       `align_information_section`, `align_to_top_border`:
 *         * Update horizontal/vertical cursors to fixed margin anchors.
 *
 *     - `align_to_right`, `align_to_right_information`,
 *       `align_to_center`:
 *         * Compute x-coordinates based on text extents so that text
 *           is right-aligned or centered within the configured widths.
 *
 *     - `adjust_height` and `adjust_payment_height`:
 *         * Predict future vertical usage and insert a page break when
 *           remaining space is insufficient to render the upcoming section.
 *
 *   Robustness:
 *     - `context_ok()` inspects the Cairo target status to detect write or
 *       surface errors after drawing operations.
 *     - All layout helpers return `bool` and are checked by callers so that
 *       `generate()` can abort gracefully and return an empty result if any
 *       drawing step fails.
 *
 *****************************************************************************/
#include <invoice_pdf.h>


namespace font_size {
        constexpr double header{50.0};
        constexpr double information{12.0};
        constexpr double prominent{15.0};
        constexpr double client_message{20.0};
}

namespace width {
        constexpr double page_width{595.0};
        constexpr double left_border{20.0};
        constexpr double right_border{575.0};
        constexpr double header{350.0};
        constexpr double information_left{40.0};
        constexpr double information_right{page_width - 40.0};
        constexpr double max_quantity{10.0};
        constexpr double max_amount{10.0};
        constexpr double left_item_description{left_border + max_quantity};
        constexpr double right_item_description{right_border - max_amount};
}

namespace height {
        constexpr double page_height{842.0};
        constexpr double top_border{60.0};
        constexpr double header{top_border + 30.0};
        constexpr double text_offset{17.0};
        constexpr double line_offset{10.0};
        constexpr double space_offset{20.0};
}


feature::invoice_pdf::invoice_pdf() {}

feature::invoice_pdf::~invoice_pdf() {}

std::string feature::invoice_pdf::generate(const std::any& _data)
{
	std::ostringstream final_pdf{};
	data::pdf_invoice data{std::any_cast<data::pdf_invoice> (_data)};
	if (data.is_valid())
	{
		this->surface = Cairo::PdfSurface::create_for_stream(
				[&, this](const unsigned char* _data, unsigned int _length) -> cairo_status_t {
				final_pdf.write(reinterpret_cast<const char*>(_data), _length);
				return final_pdf.fail() ? CAIRO_STATUS_WRITE_ERROR : CAIRO_STATUS_SUCCESS;
				},
				this->width, this->height
				);


		if (this->surface == nullptr)
		{
			return "";
		}
		else
		{
			this->context = Cairo::Context::create(this->surface);
		}

		if (this->context == nullptr)
		{
			this->surface->finish();
			return "";
		}
		else
		{
			this->context->select_font_face("monospace", Cairo::ToyFontFace::Slant::NORMAL, Cairo::ToyFontFace::Weight::NORMAL);
		}

		if (add_header("Invoice") == false)
		{
			this->context->show_page();
			this->surface->finish();
			return "";
		}

		if (add_information(data) == false)
		{
			this->context->show_page();
			this->surface->finish();
			return "";
		}

		if (add_invoice(data.get_invoice()) == false)
		{
			this->context->show_page();
			this->surface->finish();
			return "";
		}

		if (draw_line() == false)
		{
			this->context->show_page();
			this->surface->finish();
			return "";
		}

		if (add_labor(data.get_invoice()) == false)
		{
			this->context->show_page();
			this->surface->finish();
			return "";
		}

		if (draw_line() == false)
		{
			this->context->show_page();
			this->surface->finish();
			return "";
		}

		if (add_material(data.get_invoice()) == false)
		{
			this->context->show_page();
			this->surface->finish();
			return "";
		}

		if (draw_line() == false)
		{
			this->context->show_page();
			this->surface->finish();
			return "";
		}

		if (add_grand_total(data.get_invoice()) == false)
		{
			this->context->show_page();
			this->surface->finish();
			return "";
		}

		if (add_payment_method(data.get_business()) == false)
		{
			this->context->show_page();
			this->surface->finish();
			return "";
		}

		this->context->show_page();
		this->surface->finish();
	}

	return final_pdf.str();
}

bool feature::invoice_pdf::add_header(const std::string& _data)
{
        align_to_top_border();
        align_to_left_border();
        add_new_line();
        this->context->set_font_size(font_size::header);
        Cairo::TextExtents extent;
        this->context->get_text_extents(_data.c_str(), extent);
        this->context->move_to(width::header, height::header);
        this->context->show_text(_data.c_str());

        return this->context_ok();
}

bool feature::invoice_pdf::add_information(const data::pdf_invoice& _data)
{
        data::admin admin{_data.get_business()};
        data::client client{_data.get_client()};
        align_information_section();
        add_new_section();
        std::string client_business_name{client.get_name()};
        if (write_to_pdf(client_business_name, font_size::information) == false)
                return false;

        std::string name{admin.get_name()};
        if (write_to_pdf_from_right_information(name, font_size::information) == false)
                return false;

        align_information_section();
        add_new_line();
        std::string client_business_address{client.get_address()};
        if (write_to_pdf(client_business_address, font_size::information) == false)
                return false;

        std::string address{admin.get_address()};
        if (write_to_pdf_from_right_information(address, font_size::information) == false)
                return false;

        align_information_section();
        add_new_line();
        std::string client_business_town{client.get_town() + ", " + client.get_area_code()};
        if (write_to_pdf(client_business_town, font_size::information) == false)
                return false;

        std::string town{admin.get_town() + ", " + admin.get_area_code()};
        if (write_to_pdf_from_right_information(town, font_size::information) == false)
                return false;

        align_information_section();
        add_new_line();
        std::string vat_number{"VAT #: " + client.get_vat_number()};
        if (write_to_pdf(vat_number, font_size::information) == false)
                return false;

        std::string cellphone{admin.get_cellphone()};
        if (write_to_pdf_from_right_information(cellphone, font_size::information) == false)
                return false;

        add_new_line();
        std::string email{admin.get_email()};
        if (write_to_pdf_from_right_information(email, font_size::information) == false)
                return false;

        return true;
}

bool feature::invoice_pdf::add_invoice(const data::invoice& _data)
{
        align_to_left_border();
        add_new_section();

        add_new_line();
        std::string number{"Invoice #:... " + _data.get_id()};
        if (write_to_pdf(number, font_size::information) == false)
                return false;

        add_new_line();
        std::string date{"Date:........ " + _data.get_date()};
        if (write_to_pdf(date, font_size::information) == false)
                return false;

        add_new_line();
        std::string order{"Order #:..... " + _data.get_order_number()};
        if (write_to_pdf(order, font_size::information) == false)
                return false;

        add_new_line();
        std::string job_card{"Job Card:.... " + _data.get_job_card_number()};
        if (write_to_pdf(job_card, font_size::information) == false)
                return false;

        return true;
}

bool feature::invoice_pdf::add_labor(const data::invoice& _data)
{
        align_to_left_border();
        add_new_section();
        adjust_height();
        std::string header_quantity{"Quantity"};
        if (write_to_pdf(header_quantity, font_size::prominent) == false)
                return false;

        std::string header_description{"Labor Description"};
        if (write_to_pdf_in_center(header_description, font_size::prominent) == false)
                return false;

        std::string header_amount{"Amount"};
        if (write_to_pdf_from_right(header_amount, font_size::prominent) == false)
                return false;

        if (add_items(_data.get_description_column()) == false)
                return false;

        add_new_section();
        adjust_height();
        std::string total{"Total: R " + _data.get_description_total()};
        if (write_to_pdf_from_right(total, font_size::prominent) == false)
                return false;

        return true;
}

bool feature::invoice_pdf::add_material(const data::invoice& _data)
{
        align_to_left_border();
        add_new_section();
        adjust_height();
        std::string header_quantity{"Quantity"};
        if (write_to_pdf(header_quantity, font_size::prominent) == false)
                return false;

        std::string header_description{"Material Used"};
        if (write_to_pdf_in_center(header_description, font_size::prominent) == false)
                return false;

        std::string header_amount{"Amount"};
        if (write_to_pdf_from_right(header_amount, font_size::prominent) == false)
                return false;

        if (add_items(_data.get_material_column()) == false)
                return false;

        add_new_line();
        adjust_height();
        std::string total{"Total: R " + _data.get_material_total()};
        if (write_to_pdf_from_right(total, font_size::prominent) == false)
                return false;

        return true;
}

bool feature::invoice_pdf::add_items(const std::vector<data::column>& _data)
{
        add_new_line();
        for (const auto& column : _data)
        {
                this->current_width = width::left_border;
                if (write_to_pdf(std::to_string(column.get_quantity()), font_size::information) == false)
                        return false;

                std::ostringstream amount{""};
                amount << std::fixed << std::setprecision(2) << column.get_amount();
                if (write_to_pdf_from_right("R " + amount.str(), font_size::information) == false)
                        return false;

                if (add_item_description(column) == false)
                        return false;

                add_new_line();
        }

        return true;
}

bool feature::invoice_pdf::add_item_description(const data::column& _data)
{
        std::vector<std::string> sliced_data{this->slicer.slice(_data.get_description())};
        if (sliced_data.size() >= 2)
        {
                if (std::all_of(sliced_data.cbegin(), sliced_data.cend(), [&] (const std::string& str) {
                        if (write_to_pdf_in_center(str, font_size::information) == false)
                                return false;

                        add_new_line();
                        adjust_height();

                        return true;
                }) == false)
                {
                        return false;
                }
        }
        else
        {
                if (write_to_pdf_in_center(_data.get_description(), font_size::information) == false)
                        return false;

        }

        return true;
}

bool feature::invoice_pdf::add_grand_total(const data::invoice& _data)
{
        align_to_right_border();
        add_new_section();
        adjust_height();
        std::string grand_total{"Grand Total: R " + _data.get_grand_total()};
        if (write_to_pdf_from_right(grand_total, font_size::prominent) == false)
                return false;

        return true;
}

bool feature::invoice_pdf::add_payment_method(const data::admin& _data)
{
        align_to_left_border();
        add_new_section();
        adjust_payment_height();
        std::string method{"Payment Method"};
        if (write_to_pdf(method, font_size::prominent) == false)
                return false;

        add_new_line();
        std::string bank{"Bank Name:..... " + _data.get_bank()};
        if (write_to_pdf(bank, font_size::information) == false)
                return false;

        add_new_line();
        std::string account{"Account #:..... " + _data.get_account_number()};
        if (write_to_pdf(account, font_size::information) == false)
                return false;

        add_new_line();
        std::string branch{"Branch Code:... " + _data.get_branch_code()};
        if (write_to_pdf(branch, font_size::information) == false)
                return false;

        add_new_section();
        std::string message{_data.get_client_message()};
        if (write_to_pdf(message, font_size::prominent) == false)
                return false;

        return true;
}

bool feature::invoice_pdf::write_to_pdf(const std::string& _data, const double& _font_size)
{
        this->context->set_font_size(_font_size);
        Cairo::TextExtents extent;
        this->context->get_text_extents(_data.c_str(), extent);
        this->context->move_to(this->current_width, this->current_height);
        this->context->show_text(_data.c_str());

        return this->context_ok();
}

bool feature::invoice_pdf::write_to_pdf_in_center(const std::string& _data, const double& _font_size)
{
        this->context->set_font_size(_font_size);
        Cairo::TextExtents extent;
        this->context->get_text_extents(_data.c_str(), extent);
        align_to_center(extent);
        this->context->move_to(this->current_width, this->current_height);
        this->context->show_text(_data.c_str());

        return this->context_ok();
}

bool feature::invoice_pdf::write_to_pdf_from_right(const std::string& _data, const double& _font_size)
{
        this->context->set_font_size(_font_size);
        Cairo::TextExtents extent;
        this->context->get_text_extents(_data.c_str(), extent);
        align_to_right(extent);
        this->context->move_to(this->current_width, this->current_height);
        this->context->show_text(_data.c_str());

        return this->context_ok();
}

bool feature::invoice_pdf::write_to_pdf_from_right_information(const std::string& _data, const double& _font_size)
{
        this->context->set_font_size(_font_size);
        Cairo::TextExtents extent;
        this->context->get_text_extents(_data.c_str(), extent);
        align_to_right_information(extent);
        this->context->move_to(this->current_width, this->current_height);
        this->context->show_text(_data.c_str());

        return this->context_ok();
}

bool feature::invoice_pdf::draw_line()
{
        this->current_height += height::line_offset;
        this->context->set_line_width(1.5);
        this->context->move_to(width::left_border, this->current_height);
        this->context->line_to((this->width - width::left_border), this->current_height);
        this->context->stroke();

        return this->context_ok();
}

bool feature::invoice_pdf::context_ok()
{
        return (context->get_target()->get_status() == CAIRO_STATUS_SUCCESS);
}

void feature::invoice_pdf::add_new_line()
{
	double next_line = this->current_height + height::text_offset;
	if (next_line >= (height::page_height - height::top_border)) {
		this->context->show_page();
		align_to_top_border();
		align_to_left_border();
	}

	std::lock_guard<std::mutex> guard(this->pdf_mutex);
	this->current_height += height::text_offset;
}

void feature::invoice_pdf::add_new_section()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_height += height::space_offset;
        this->current_height += height::space_offset;
}

void feature::invoice_pdf::align_to_left_border()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_width = width::left_border;
}

void feature::invoice_pdf::align_to_right_border()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_width = width::right_border;
}

void feature::invoice_pdf::align_information_section()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_width = width::information_left;
}

void feature::invoice_pdf::align_to_top_border()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_height = height::top_border;
}

void feature::invoice_pdf::align_to_right(const Cairo::TextExtents& _extent)
{
        this->current_width = (width::right_border - (_extent.width + _extent.x_bearing));
}

void feature::invoice_pdf::align_to_right_information(const Cairo::TextExtents& _extent)
{
        this->current_width = (width::information_right - (_extent.width + _extent.x_bearing));
}

void feature::invoice_pdf::align_to_center(const cairo_text_extents_t& _extent)
{
        this->current_width = ((this->width / 2) - ((_extent.width / 2) + _extent.x_bearing));
}

void feature::invoice_pdf::adjust_height()
{
        if (this->current_height >= (height::page_height - (height::top_border)))
        {
                this->context->show_page();
                align_to_top_border();
        }
}

void feature::invoice_pdf::adjust_payment_height()
{
        double future_height{this->current_height + (height::space_offset * 5) + (height::text_offset * 2)};
        if (future_height >= (height::page_height - (height::top_border)))
        {
                this->context->show_page();
                align_to_top_border();
        }
}
