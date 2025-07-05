/*******************************************************************************
 * Contents: Sending emails feature unit tests
 * Author: Dawid Blom
 * Date: December 23, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <iostream>

#include <invoice_pdf.h>
#include <email.h>
#include <email_data.h>
#include <client_data.h>
#include <invoice_data.h>
#include <business_data.h>
#include <pdf_invoice_data.h>
#include <generate_pdf.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1)
 ******************************************************************************/
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

        data::business business_data{test::generate_business_data()};
        pdf_data.set_business(business_data);

        data::client client_data{test::generate_client_data()};
        pdf_data.set_client(client_data);

        data::invoice invoice_data{test::generate_invoice_data(short_description)};
        pdf_data.set_invoice(invoice_data);

        feature::invoice_pdf invoice_pdf{};
        std::vector<std::string> pdf_file_data{invoice_pdf.generate_for_email(pdf_data)};
        data.set_attachments(pdf_file_data);
        data.set_client(client_data);
        data.set_business(business_data);
        data.set_subject("Invoice");


        CHECK_EQUAL(true, email.send(data));
}

TEST(email_test, send_multiple_attachments)
{
        data::pdf_invoice pdf_data;

        data::business business_data{test::generate_business_data()};
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
                std::string pdf_file_data{invoice_pdf.generate_for_email(pdf_data)};
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

        data::business business_data;
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
