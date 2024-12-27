/********************************************************
 * Contents: PDF definition
 * Author: Dawid J. Blom
 * Date: December 23, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _PDF_H_
#define _PDF_H_
#include <string>
#include <ostream>
#include <errors.h>
#include <cairo/cairo.h>
#include <business_data.h>
#include <cairo/cairo-pdf.h>
#include <pdf_invoice_data.h>

namespace feature {
        class pdf {
                public:
                        pdf();
                        pdf(const pdf&) = delete;
                        pdf(pdf&&) = delete;
                        pdf& operator = (const pdf&) = delete;
                        pdf& operator = (pdf&&) = delete;
                        virtual ~pdf();

                        [[nodiscard]] virtual std::string generate(data::pdf_invoice&);

                private:
                        [[nodiscard]] bool add_header(const std::string&);
                        [[nodiscard]] bool add_business(const data::business&);
                        [[nodiscard]] bool context_ok();
                        static cairo_status_t write_to_stream(void*, const unsigned char*, unsigned int);

                private:
                        const double width{595.0};
                        const double height{842.0};
                        double current_width{0.0};
                        double current_height{0.0};
                        cairo_surface_t* surface{nullptr};
                        cairo_t* context{nullptr};
                        std::ostringstream final_pdf{""};
        };
}
#endif
