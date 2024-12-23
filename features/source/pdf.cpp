/********************************************************
 * Contents: PDF implementation
 * Author: Dawid J. Blom
 * Date: December 23, 2024
 *
 * NOTE:
 *******************************************************/
#include <pdf.h>


feature::pdf::pdf()
{
        cairo_surface_t* surface = cairo_pdf_surface_create_for_stream(write_to_stream, &final_pdf, 595, 842);
        if (cairo_surface_status(surface) != CAIRO_STATUS_SUCCESS)
                throw app::errors::construction;

}

feature::pdf::~pdf()
{

}

cairo_status_t feature::pdf::write_to_stream(void* closure, const unsigned char* data, unsigned int length)
{
        std::ostream* out = static_cast<std::ostream*>(closure);
        if (!out->write(reinterpret_cast<const char*>(data), length))
        {
                return CAIRO_STATUS_WRITE_ERROR;
        }

        return CAIRO_STATUS_SUCCESS;
}
