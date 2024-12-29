/********************************************************
 * Contents: PDF implementation
 * Author: Dawid J. Blom
 * Date: December 23, 2024
 *
 * NOTE:
 *******************************************************/
#include <pdf.h>


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
        constexpr double information{40.0};
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

feature::pdf::pdf()
{
        this->surface = cairo_pdf_surface_create_for_stream(write_to_stream, &this->final_pdf, this->width, this->height);
        if (cairo_surface_status(this->surface) != CAIRO_STATUS_SUCCESS) //GCOVR_EXCL_START
                throw app::errors::construction;

        this->context = cairo_create(this->surface);
        if (cairo_status(this->context) != CAIRO_STATUS_SUCCESS)
                throw app::errors::construction;

        cairo_select_font_face(this->context, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        if (cairo_status(this->context) != CAIRO_STATUS_SUCCESS)
                throw app::errors::construction;
} //GCOVR_EXCL_STOP

feature::pdf::~pdf()
{
        cairo_destroy(this->context);
        cairo_surface_finish(this->surface);
        cairo_surface_destroy(this->surface);
}

std::string feature::pdf::generate(data::pdf_invoice& _data)
{
        if (_data.is_valid())
        {
                if (add_header("Invoice") == false)
                        return "";

                if (add_business(_data.get_business()) == false)
                        return "";

                if (add_client(_data.get_client()) == false)
                        return "";

                if (add_invoice(_data.get_invoice()) == false)
                        return "";

                if (draw_line() == false)
                        return "";

                if (add_labor(_data.get_invoice()) == false)
                        return "";

                if (draw_line() == false)
                        return "";

                if (add_material(_data.get_invoice()) == false)
                        return "";

                if (draw_line() == false)
                        return "";

                if (add_grand_total(_data.get_invoice()) == false)
                        return "";

                if (add_payment_method(_data.get_business()) == false)
                        return "";

                cairo_show_page(this->context);
                cairo_surface_finish(this->surface);
        }


        return this->final_pdf.str();
}

bool feature::pdf::add_header(const std::string& _data)
{
        align_to_left_border();
        add_new_line();
        cairo_set_font_size(this->context, font_size::header);
        cairo_text_extents_t extent;
        cairo_text_extents(this->context, _data.c_str(), &extent);
        cairo_move_to(this->context, width::header, height::header);
        cairo_show_text(this->context, _data.c_str());

        return this->context_ok();
}

bool feature::pdf::add_business(const data::business& _data)
{
        align_information_section();
        align_to_top_border();
        std::string name{_data.get_name()};
        if (write_to_pdf(name, font_size::information) == false)
                return false;

        add_new_line();
        std::string address{_data.get_address()};
        if (write_to_pdf(address, font_size::information) == false)
                return false;

        add_new_line();
        std::string town{_data.get_town() + ", " + _data.get_area_code()};
        if (write_to_pdf(town, font_size::information) == false)
                return false;

        add_new_line();
        std::string cellphone{_data.get_cellphone()};
        if (write_to_pdf(cellphone, font_size::information) == false)
                return false;

        add_new_line();
        std::string email{_data.get_email()};
        if (write_to_pdf(email, font_size::information) == false)
                return false;

        return true;
}

bool feature::pdf::add_client(const data::client& _data)
{
        align_information_section();
        add_new_section();
        std::string name{_data.get_business_name()};
        if (write_to_pdf(name, font_size::information) == false)
                return false;

        add_new_line();
        std::string address{_data.get_business_address()};
        if (write_to_pdf(address, font_size::information) == false)
                return false;

        add_new_line();
        std::string town{_data.get_business_town_name() + ", " + _data.get_business_area_code()};
        if (write_to_pdf(town, font_size::information) == false)
                return false;

        add_new_line();
        std::string vat_number{"VAT #: " + _data.get_vat_number()};
        if (write_to_pdf(vat_number, font_size::information) == false)
                return false;

        return true;
}

bool feature::pdf::add_invoice(const data::invoice& _data)
{
        align_to_left_border();
        add_new_section();

        add_new_line();
        std::string number{"Invoice #:... " + _data.get_invoice_number()};
        if (write_to_pdf(number, font_size::information) == false)
                return false;

        add_new_line();
        std::string date{"Date:........ " + _data.get_invoice_date()};
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

bool feature::pdf::add_labor(const data::invoice& _data)
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

bool feature::pdf::add_material(const data::invoice& _data)
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

bool feature::pdf::add_items(const std::vector<data::column>& _data)
{
        add_new_line();
        for (const auto& column : _data)
        {
                this->current_width = width::left_border;
                if (write_to_pdf(std::to_string(column.get_quantity()), font_size::information) == false)
                        return false;

                std::ostringstream amount{""};
                amount << std::fixed << std::setprecision(2) << column.get_amount();
                if (write_to_pdf_from_right("R " + amount.str() , font_size::information) == false)
                        return false;

                if (add_item_description(column) == false)
                        return false;

                add_new_line();
        }

        return true;
}

bool feature::pdf::add_item_description(const data::column& _data)
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

bool feature::pdf::add_grand_total(const data::invoice& _data)
{
        align_to_right_border();
        add_new_section();
        adjust_height();
        std::string grand_total{"Grand Total: R " + _data.get_grand_total()};
        if (write_to_pdf_from_right(grand_total, font_size::prominent) == false)
                return false;

        return true;
}

bool feature::pdf::add_payment_method(const data::business& _data)
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

bool feature::pdf::write_to_pdf(const std::string& _data, const double& _font_size)
{
        cairo_set_font_size(this->context, _font_size);
        cairo_text_extents_t extent;
        cairo_text_extents(this->context, _data.c_str(), &extent);
        cairo_move_to(this->context, this->current_width, this->current_height);
        cairo_show_text(this->context, _data.c_str());

        return this->context_ok();
}

bool feature::pdf::write_to_pdf_from_right(const std::string& _data, const double& _font_size)
{
        cairo_set_font_size(this->context, _font_size);
        cairo_text_extents_t extent;
        cairo_text_extents(this->context, _data.c_str(), &extent);
        align_to_right(extent);
        cairo_move_to(this->context, this->current_width, this->current_height);
        cairo_show_text(this->context, _data.c_str());

        return this->context_ok();
}

bool feature::pdf::write_to_pdf_in_center(const std::string& _data, const double& _font_size)
{
        cairo_set_font_size(this->context, _font_size);
        cairo_text_extents_t extent;
        cairo_text_extents(this->context, _data.c_str(), &extent);
        align_to_center(extent);
        cairo_move_to(this->context, this->current_width, this->current_height);
        cairo_show_text(this->context, _data.c_str());

        return this->context_ok();
}

bool feature::pdf::draw_line()
{
        this->current_height += height::line_offset;
        cairo_set_line_width(this->context, 1.5);
        cairo_move_to(this->context, width::left_border, this->current_height);
        cairo_line_to(this->context, (this->width - width::left_border), this->current_height);
        cairo_stroke(this->context);

        return this->context_ok();
}

bool feature::pdf::context_ok()
{
        return (cairo_status(this->context) == CAIRO_STATUS_SUCCESS);
}

void feature::pdf::add_new_line()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_height += height::text_offset;
}

void feature::pdf::add_new_section()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_height += height::space_offset;
        this->current_height += height::space_offset;
}

void feature::pdf::align_to_left_border()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_width = width::left_border;
}

void feature::pdf::align_to_right_border()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_width = width::right_border;
}

void feature::pdf::align_information_section()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_width = width::information;
}

void feature::pdf::align_to_top_border()
{
        std::lock_guard<std::mutex> guard(this->pdf_mutex);
        this->current_height = height::top_border;
}

void feature::pdf::align_to_right(const cairo_text_extents_t& _extent)
{
        this->current_width = (width::right_border - (_extent.width + _extent.x_bearing));
}

void feature::pdf::align_to_center(const cairo_text_extents_t& _extent)
{
        this->current_width = ((this->width / 2) - ((_extent.width / 2) + _extent.x_bearing));
}

void feature::pdf::adjust_height()
{
        if (this->current_height >= (height::page_height - (height::top_border)))
        {
                cairo_show_page(this->context);
                align_to_top_border();
        }
}

void feature::pdf::adjust_payment_height()
{
        double future_height{this->current_height + (height::space_offset * 5) + (height::text_offset * 2)};
        if (future_height >= (height::page_height - (height::top_border)))
        {
                cairo_show_page(this->context);
                align_to_top_border();
        }
}

cairo_status_t feature::pdf::write_to_stream(void* _closure, const unsigned char* _data, unsigned int _length)
{
        if (!_closure && !_data)
                return CAIRO_STATUS_WRITE_ERROR;

        std::ostream* out = static_cast<std::ostream*>(_closure);
        if (!out->write(reinterpret_cast<const char*>(_data), _length))
                return CAIRO_STATUS_WRITE_ERROR;

        return CAIRO_STATUS_SUCCESS;
}
