/********************************************************
 * Contents: statement_pdf implementation
 * Author: Dawid J. Blom
 * Date: July 11, 2025
 *
 * NOTE:
 *******************************************************/
#include <statement_pdf.h>

namespace font_size {
        constexpr double header{50.0};
        constexpr double information{12.0};
        constexpr double prominent{15.0};
}

namespace width {
        constexpr double page_width{595.0};
        constexpr double left_border{20.0};
        constexpr double right_border{575.0};
        constexpr double header{290.0};
        constexpr double information_left{40.0};
        constexpr double information_right{page_width - 40.0};
}

namespace height {
        constexpr double page_height{842.0};
        constexpr double top_border{60.0};
        constexpr double header{top_border + 30.0};
        constexpr double text_offset{17.0};
        constexpr double line_offset{10.0};
        constexpr double space_offset{20.0};
}


feature::statement_pdf::statement_pdf() {}

feature::statement_pdf::~statement_pdf() {}

std::string feature::statement_pdf::generate(const std::any& _data)
{
        std::ostringstream final_pdf{};
	data::pdf_statement data{std::any_cast<data::pdf_statement> (_data)};
        if (data.is_valid() == false)
	{
                syslog(LOG_CRIT, "Data is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
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
                        this->context->select_font_face("monospace", Cairo::ToyFontFace::Slant::NORMAL, Cairo::ToyFontFace::Weight::NORMAL);

                if (add_header("Statement") == false)
		{
			this->context->show_page();
			this->surface->finish();
			return "";
		}

		data::client client{(data.get_pdf_invoices())[0].get_client()};
		data::admin admin{(data.get_pdf_invoices())[0].get_business()};
                if (add_information(client, admin) == false)
		{
			this->context->show_page();
			this->surface->finish();
			return "";
		}

                if (add_statement_information(data) == false)
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

                if (add_statements(data) == false)
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

                if (add_grand_total(data) == false)
		{
			this->context->show_page();
			this->surface->finish();
			return "";
		}

                if (add_payment_method((data.get_pdf_invoices())[0].get_business()) == false)
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

bool feature::statement_pdf::add_header(const std::string& _data)
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

bool feature::statement_pdf::add_information(const data::client& _client, const data::admin& _business)
{
        align_information_section();
        add_new_section();
        std::string client_business_name{_client.get_name()};
        if (write_to_pdf(client_business_name, font_size::information) == false)
                return false;

        std::string name{_business.get_name()};
        if (write_to_pdf_from_right_information(name, font_size::information) == false)
                return false;

        align_information_section();
        add_new_line();
        std::string client_business_address{_client.get_address()};
        if (write_to_pdf(client_business_address, font_size::information) == false)
                return false;

        std::string address{_business.get_address()};
        if (write_to_pdf_from_right_information(address, font_size::information) == false)
                return false;

        align_information_section();
        add_new_line();
        std::string client_business_town{_client.get_town() + ", " + _client.get_area_code()};
        if (write_to_pdf(client_business_town, font_size::information) == false)
                return false;

        std::string town{_business.get_town() + ", " + _business.get_area_code()};
        if (write_to_pdf_from_right_information(town, font_size::information) == false)
                return false;

        align_information_section();
        add_new_line();
        std::string vat_number{"VAT #: " + _client.get_vat_number()};
        if (write_to_pdf(vat_number, font_size::information) == false)
                return false;

        std::string cellphone{_business.get_cellphone()};
        if (write_to_pdf_from_right_information(cellphone, font_size::information) == false)
                return false;

        add_new_line();
        std::string email{_business.get_email()};
        if (write_to_pdf_from_right_information(email, font_size::information) == false)
                return false;

        return true;
}

bool feature::statement_pdf::add_statement_information(const data::pdf_statement& _data)
{
        align_to_left_border();
        add_new_section();

        add_new_line();
        std::string number{"Statement #:... " + _data.get_number()};
        if (write_to_pdf(number, font_size::information) == false)
                return false;

        add_new_line();
        std::string date{"Date:.......... " + _data.get_date()};
        if (write_to_pdf(date, font_size::information) == false)
                return false;

        return true;
}

bool feature::statement_pdf::add_statements(const data::pdf_statement& _data)
{
        align_to_left_border();
        add_new_section();
        adjust_height();
        std::string header_invoice_number{"Invoice #"};
        if (write_to_pdf(header_invoice_number, font_size::prominent) == false)
                return false;

        std::string header_date{"Date"};
	if (write_to_pdf_in_first_quarter(header_date, font_size::prominent) == false)
		return false;

        std::string header_order_number{"Order #"};
        if (write_to_pdf_in_center(header_order_number, font_size::prominent) == false)
                return false;

        std::string header_paid_status{"Paid Status"};
        if (write_to_pdf_in_second_quarter(header_paid_status, font_size::prominent) == false)
                return false;

        std::string header_price{"Price"};
        if (write_to_pdf_from_right(header_price, font_size::prominent) == false)
                return false;

        if (add_items(_data.get_pdf_invoices()) == false)
                return false;

        return true;
}

bool feature::statement_pdf::add_items(const std::vector<data::pdf_invoice>& _data)
{
        add_new_line();
        add_new_line();
        for (const data::pdf_invoice& pdf_invoice : _data)
        {
		data::invoice invoice{pdf_invoice.get_invoice()};
                this->current_width = width::left_border;
                if (write_to_pdf("# " + invoice.get_invoice_number(), font_size::information) == false)
			break;

                if (write_to_pdf_in_first_quarter(invoice.get_invoice_date() , font_size::information) == false)
			break;

                if (write_to_pdf_in_second_quarter(invoice.get_paid_status() , font_size::information) == false)
			break;

                if (write_to_pdf_from_right("R " + invoice.get_grand_total(), font_size::information) == false)
                // if (write_to_pdf_from_right("R " + goss.str(), font_size::information) == false)
			break;

		std::vector<std::string> sliced_data{this->slicer.slice(invoice.get_order_number())};
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
			if (write_to_pdf_in_center(invoice.get_order_number() , font_size::information) == false)
				return false;
		}

                add_new_line();
        }

        return true;
}

bool feature::statement_pdf::add_grand_total(const data::pdf_statement& _data)
{
        align_to_right_border();
        add_new_section();
        adjust_height();
        std::string grand_total{"Grand Total: R " + _data.get_total()};
        if (write_to_pdf_from_right(grand_total, font_size::prominent) == false)
                return false;

        return true;
}

bool feature::statement_pdf::add_payment_method(const data::admin& _data)
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

bool feature::statement_pdf::write_to_pdf(const std::string& _data, const double& _font_size)
{
        this->context->set_font_size(_font_size);
        Cairo::TextExtents extent;
        this->context->get_text_extents(_data.c_str(), extent);
        this->context->move_to(this->current_width, this->current_height);
        this->context->show_text(_data.c_str());

        return this->context_ok();
}

bool feature::statement_pdf::write_to_pdf_in_first_quarter(const std::string& _data, const double& _font_size)
{
        this->context->set_font_size(_font_size);
        Cairo::TextExtents extent;
        this->context->get_text_extents(_data.c_str(), extent);
        align_to_first_quarter(extent);
        this->context->move_to(this->current_width, this->current_height);
        this->context->show_text(_data.c_str());

        return this->context_ok();
}

bool feature::statement_pdf::write_to_pdf_in_center(const std::string& _data, const double& _font_size)
{
        this->context->set_font_size(_font_size);
        Cairo::TextExtents extent;
        this->context->get_text_extents(_data.c_str(), extent);
        align_to_center(extent);
        this->context->move_to(this->current_width, this->current_height);
        this->context->show_text(_data.c_str());

        return this->context_ok();
}

bool feature::statement_pdf::write_to_pdf_in_second_quarter(const std::string& _data, const double& _font_size)
{
        this->context->set_font_size(_font_size);
        Cairo::TextExtents extent;
        this->context->get_text_extents(_data.c_str(), extent);
        align_to_second_quarter(extent);
        this->context->move_to(this->current_width, this->current_height);
        this->context->show_text(_data.c_str());

        return this->context_ok();
}

bool feature::statement_pdf::write_to_pdf_from_right(const std::string& _data, const double& _font_size)
{
        this->context->set_font_size(_font_size);
        Cairo::TextExtents extent;
        this->context->get_text_extents(_data.c_str(), extent);
        align_to_right(extent);
        this->context->move_to(this->current_width, this->current_height);
        this->context->show_text(_data.c_str());

        return this->context_ok();
}

bool feature::statement_pdf::write_to_pdf_from_right_information(const std::string& _data, const double& _font_size)
{
        this->context->set_font_size(_font_size);
        Cairo::TextExtents extent;
        this->context->get_text_extents(_data.c_str(), extent);
        align_to_right_information(extent);
        this->context->move_to(this->current_width, this->current_height);
        this->context->show_text(_data.c_str());

        return this->context_ok();
}

bool feature::statement_pdf::draw_line()
{
        this->current_height += height::line_offset;
        this->context->set_line_width(1.5);
        this->context->move_to(width::left_border, this->current_height);
        this->context->line_to((this->width - width::left_border), this->current_height);
        this->context->stroke();

        return this->context_ok();
}

bool feature::statement_pdf::context_ok()
{
        return (context->get_target()->get_status() == CAIRO_STATUS_SUCCESS);
}

void feature::statement_pdf::add_new_line()
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

void feature::statement_pdf::add_new_section()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_height += height::space_offset;
        this->current_height += height::space_offset;
}

void feature::statement_pdf::align_to_left_border()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_width = width::left_border;
}

void feature::statement_pdf::align_to_right_border()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_width = width::right_border;
}

void feature::statement_pdf::align_information_section()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_width = width::information_left;
}

void feature::statement_pdf::align_to_top_border()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_height = height::top_border;
}

void feature::statement_pdf::align_to_right(const Cairo::TextExtents& _extent)
{
        this->current_width = (width::right_border - (_extent.width + _extent.x_bearing));
}

void feature::statement_pdf::align_to_right_information(const Cairo::TextExtents& _extent)
{
        this->current_width = (width::information_right - (_extent.width + _extent.x_bearing));
}

void feature::statement_pdf::align_to_first_quarter(const cairo_text_extents_t& _extent)
{
        this->current_width = ((this->width / 4) - ((_extent.width / 4) + (_extent.x_bearing - 10)));
}

void feature::statement_pdf::align_to_center(const cairo_text_extents_t& _extent)
{
        this->current_width = ((this->width / 2) - ((_extent.width / 2) + _extent.x_bearing));
}

void feature::statement_pdf::align_to_second_quarter(const cairo_text_extents_t& _extent)
{
        this->current_width = ((this->width / 4) - ((_extent.width / 4) + (_extent.x_bearing - 250)));
}

void feature::statement_pdf::adjust_height()
{
        if (this->current_height >= (height::page_height - (height::top_border)))
        {
                this->context->show_page();
                align_to_top_border();
        }
}

void feature::statement_pdf::adjust_payment_height()
{
        double future_height{this->current_height + (height::space_offset * 5) + (height::text_offset * 2)};
        if (future_height >= (height::page_height - (height::top_border)))
        {
                this->context->show_page();
                align_to_top_border();
        }
}
