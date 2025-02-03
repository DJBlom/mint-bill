/*******************************************************************************
 * Contents: Sending emails feature unit tests
 * Author: Dawid Blom
 * Date: December 23, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <pdf.h>
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

        feature::pdf pdf{};
        std::string pdf_file_data{pdf.generate_for_email(pdf_data)};
        data.set_pdf(pdf_file_data);
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

        feature::pdf pdf{};
        std::string pdf_file_data;
        data.set_pdf(pdf_file_data);
        data.set_client(client_data);
        data.set_business(business_data);
        data.set_subject("");


        CHECK_EQUAL(false, email.send(data));
}
