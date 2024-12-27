/********************************************************
 * Contents: PDF implementation
 * Author: Dawid J. Blom
 * Date: December 23, 2024
 *
 * NOTE:
 *******************************************************/
#include <pdf.h>
#include <iostream>


namespace width {
        constexpr double border{20.0};
        constexpr double header{300.0};
}

namespace height {
        constexpr double border{30.0};
        constexpr double header{80.0};
}

feature::pdf::pdf()
{
        this->surface = cairo_pdf_surface_create_for_stream(write_to_stream, &this->final_pdf, this->width, this->height);
        if (cairo_surface_status(this->surface) != CAIRO_STATUS_SUCCESS)
                throw app::errors::construction;

        this->context = cairo_create(this->surface);
        if (cairo_status(this->context) != CAIRO_STATUS_SUCCESS)
                throw app::errors::construction;

        cairo_select_font_face(this->context, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        if (cairo_status(this->context) != CAIRO_STATUS_SUCCESS)
                throw app::errors::construction;
}

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

                cairo_show_page(this->context);
                cairo_surface_finish(this->surface);
        }


        return this->final_pdf.str();
}

bool feature::pdf::add_header(const std::string& _data)
{
        cairo_set_font_size(this->context, 50);
        cairo_text_extents_t extent;
        cairo_text_extents(this->context, _data.c_str(), &extent);

        cairo_move_to(this->context, width::header, height::header);
        cairo_show_text(this->context, _data.c_str());

        return this->context_ok();
}

bool feature::pdf::add_business(const data::business& _data)
{
        bool added{false};
        if (_data.is_valid())
        {
                added = true;
        }

        return added;
}

bool feature::pdf::context_ok()
{
        return (cairo_status(this->context) == CAIRO_STATUS_SUCCESS);
}

cairo_status_t feature::pdf::write_to_stream(void* _closure, const unsigned char* _data, unsigned int _length)
{
        if (!_closure && !_data)
                return CAIRO_STATUS_WRITE_ERROR;

        std::ostream* out = static_cast<std::ostream*>(_closure);
        if (!out->write(reinterpret_cast<const char*>(_data), _length))
        {
                return CAIRO_STATUS_WRITE_ERROR;
        }

        return CAIRO_STATUS_SUCCESS;
}
