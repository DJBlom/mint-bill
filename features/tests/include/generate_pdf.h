/********************************************************
 * Contents: Generate dummy pdf definition
 * Author: Dawid J. Blom
 * Date: December 30, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _TEST_GENERATE_PDF_H_
#define _TEST_GENERATE_PDF_H_
#include <fstream>
#include <client_data.h>
#include <invoice_data.h>
#include <business_data.h>

namespace test {
        bool generate_invoice_pdf(const std::string&, const std::string&);
        data::business generate_business_data();
        data::client generate_client_data();
        data::client generate_client_data_multiple_emails();
        data::invoice generate_invoice_data(const std::string&);
}
#endif
