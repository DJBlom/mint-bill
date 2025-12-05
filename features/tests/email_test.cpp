/******************************************************************************
 * @test_list Email Feature Test Suite
 *
 * @brief
 *   This test suite validates the behavior of the e-mail sending feature,
 *   including end-to-end integration with PDF generation, attachment handling,
 *   and data validation logic.
 *
 * @details
 *   The following behaviors are tested:
 *
 *   1. **Successful e-mail sending with valid data**
 *      - Ensures that when all required business, client, and invoice fields
 *        are valid, and a correct PDF attachment is provided, the e-mail
 *        subsystem successfully transmits the message.
 *
 *   2. **Handling of multiple PDF attachments**
 *      - Verifies that the system can attach and transmit multiple generated
 *        invoice PDFs and still report a successful send operation.
 *
 *   3. **Failure on invalid or missing data**
 *      - Confirms that the feature prevents sending when required fields are
 *        missing, blank, or invalid (e.g., missing subject, empty attachments,
 *        invalid business/client data).
 *
 *   Together, these tests ensure correct functionality of the e-mail sending
 *   pipeline under normal, stress, and error conditions.
 *
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <iostream>

#include <invoice_pdf.h>
#include <email.h>
#include <email_data.h>
#include <client_data.h>
#include <invoice_data.h>
#include <admin_data.h>
#include <pdf_invoice_data.h>
#include <generate_pdf.h>
extern "C"
{

}


TEST_GROUP(email_test)
{
        data::email data;
        feature::email email{};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(email_test, send_data_under_good_conditions)
{
        std::string short_description{"Machining steel"};
        data::pdf_invoice pdf_data;

        data::admin business_data{test::generate_business_data()};
        pdf_data.set_business(business_data);

        data::client client_data{test::generate_client_data()};
        pdf_data.set_client(client_data);

        data::invoice invoice_data{test::generate_invoice_data(short_description)};
        pdf_data.set_invoice(invoice_data);

        feature::invoice_pdf invoice_pdf{};
        std::vector<std::string> pdf_file_data{invoice_pdf.generate(pdf_data)};
        data.set_attachments(pdf_file_data);
        data.set_client(client_data);
        data.set_business(business_data);
        data.set_subject("Invoice");


        CHECK_EQUAL(true, email.send(data));
}

TEST(email_test, send_multiple_attachments)
{
        data::pdf_invoice pdf_data;

        data::admin business_data{test::generate_business_data()};
        pdf_data.set_business(business_data);

        data::client client_data{test::generate_client_data()};
        pdf_data.set_client(client_data);

        std::vector<std::string> invoice_attachments{3};
        for (int i = 0; i < 3; ++i)
        {
                feature::invoice_pdf invoice_pdf{};
                std::string short_description{"Machining steel "};
                data::invoice invoice_data{test::generate_invoice_data(short_description, i)};
                pdf_data.set_invoice(invoice_data);
                std::string pdf_file_data{invoice_pdf.generate(pdf_data)};
                test::generate_invoice_pdf(pdf_file_data, std::to_string(i));
                invoice_attachments.push_back(pdf_file_data);
        }

        data.set_attachments(invoice_attachments);
        data.set_client(client_data);
        data.set_business(business_data);
        data.set_subject("Invoice");


        CHECK_EQUAL(true, email.send(data));
}

TEST(email_test, send_no_data)
{
        std::string short_description{""};
        data::pdf_invoice pdf_data;

        data::admin business_data;
        pdf_data.set_business(business_data);

        data::client client_data;
        pdf_data.set_client(client_data);

        data::invoice invoice_data;
        pdf_data.set_invoice(invoice_data);

        feature::invoice_pdf invoice_pdf{};
        std::vector<std::string> invoice_attachments;
        data.set_attachments(invoice_attachments);
        data.set_client(client_data);
        data.set_business(business_data);
        data.set_subject("");


        CHECK_EQUAL(false, email.send(data));
}
