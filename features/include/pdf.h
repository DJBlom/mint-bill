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
#include <cairo/cairo-pdf.h>

namespace feature {
        class pdf {
                public:
                        pdf();
                        pdf(const pdf&) = delete;
                        pdf(pdf&&) = delete;
                        pdf& operator = (const pdf&) = delete;
                        pdf& operator = (pdf&&) = delete;
                        virtual ~pdf();

                private:
                        static cairo_status_t write_to_stream(void*, const unsigned char*, unsigned int);

                private:
                        std::string final_pdf{""};
        };
}
#endif
