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
#include <vector>
#include <ostream>
#include <sstream>
#include <errors.h>
#include <cairo/cairo.h>
#include <client_data.h>
#include <invoice_data.h>
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
                        [[nodiscard]] bool add_client(const data::client&);
                        [[nodiscard]] bool add_invoice(const data::invoice&);
                        [[nodiscard]] bool add_labor(const data::invoice&);
                        [[nodiscard]] bool add_material(const data::invoice&);
                        [[nodiscard]] bool add_items(const std::vector<data::column>&);
                        [[nodiscard]] bool add_grand_total(const data::invoice&);
                        [[nodiscard]] bool add_payment_method(const data::business&);
                        [[nodiscard]] bool write_to_pdf_from_left(const std::string&, const double&);
                        [[nodiscard]] bool write_to_pdf_in_center(const std::string&, const double&);
                        [[nodiscard]] bool write_to_pdf_from_right(const std::string&, const double&);
                        [[nodiscard]] bool draw_line();
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
