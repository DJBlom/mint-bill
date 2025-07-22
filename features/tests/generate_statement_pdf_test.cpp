/*******************************************************************************
 * Contents: Generate statement_pdf feature unit tests
 * Author: Dawid Blom
 * Date: July 11, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <gtkmm.h>
#include <fstream>
#include <client_data.h>
#include <invoice_data.h>
#include <generate_pdf.h>
#include <business_data.h>
#include <statement_pdf.h>
#include <pdf_statement_data.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Inovice PDFs are generated according to the requirements. (Done)
 * 2) PDFs must be creatable from different threads. (Done)
 ******************************************************************************/
TEST_GROUP(generate_statement_pdf_test)
{
        feature::statement_pdf statement_pdf;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(generate_statement_pdf_test, generate_statement_pdf_for_upper_bounds)
{
        std::string long_description{"Beneath the sky so vast and blue, Where whispers weave the morning dew, The trees stand tall, their shadows play, A dance of life, a soft ballet. The river hums a timeless tune, Reflecting light from sun to moon, It carries dreams to oceans deep, A secret world where hopes may sleep. The stars ignite the velvet night, Their spark, a beacon’s gentle light. Through every breath, through joy and strife, We live, we dream—the dance of life. Forever flows this fleeting glow. Test completed, good!"};

	std::vector<data::pdf_invoice> pdf_invoices;
	for (int i = 0; i < 50; ++i)
	{
		data::pdf_invoice pdf_invoice;
		data::business business_data{test::generate_business_data()};
		pdf_invoice.set_business(business_data);
		data::client client_data{test::generate_client_data()};
		pdf_invoice.set_client(client_data);
		data::invoice invoice_data{test::generate_invoice_data(long_description)};
		pdf_invoice.set_invoice(invoice_data);
		pdf_invoices.push_back(pdf_invoice);
	}
	data::pdf_statement pdf_statement{};
	pdf_statement.set_number("#1");
	pdf_statement.set_date("2025-06-14");
	pdf_statement.set_total("1234567898.00");
	pdf_statement.set_pdf_invoices(pdf_invoices);
        std::string pdf_file_data{statement_pdf.generate(pdf_statement)};
        bool result{test::generate_statement_pdf(pdf_file_data, "1")};

        CHECK_EQUAL(true, result);
}

TEST(generate_statement_pdf_test, generate_statement_pdf_for_lower_bounds)
{
        std::string short_description{"Machining steel"};
	std::vector<data::pdf_invoice> pdf_invoices;
	for (int i = 0; i < 50; ++i)
	{
		data::pdf_invoice pdf_invoice;
		data::business business_data{test::generate_business_data()};
		pdf_invoice.set_business(business_data);
		data::client client_data{test::generate_client_data()};
		pdf_invoice.set_client(client_data);
		data::invoice invoice_data{test::generate_invoice_data(short_description)};
		pdf_invoice.set_invoice(invoice_data);
		pdf_invoices.push_back(pdf_invoice);
	}
	data::pdf_statement pdf_statement{};
	pdf_statement.set_number("#1");
	pdf_statement.set_date("2025-06-14");
	pdf_statement.set_total("1234567898.00");
	pdf_statement.set_pdf_invoices(pdf_invoices);
        std::string pdf_file_data{statement_pdf.generate(pdf_statement)};
        bool result{test::generate_statement_pdf(pdf_file_data, "2")};

        CHECK_EQUAL(true, result);
}

TEST(generate_statement_pdf_test, generate_statement_pdf_for_no_data)
{
        std::string short_description{"Machining steel"};
	std::vector<data::pdf_invoice> pdf_invoices;
	data::pdf_invoice pdf_invoice;
        data::business business_data{};
        pdf_invoice.set_business(business_data);
        data::client client_data{};
        pdf_invoice.set_client(client_data);
        data::invoice invoice_data{};
        pdf_invoice.set_invoice(invoice_data);
	pdf_invoices.push_back(pdf_invoice);
	data::pdf_statement pdf_statement{};
	pdf_statement.set_number("#1");
	pdf_statement.set_date("");
	pdf_statement.set_total("1234567898.00");
	pdf_statement.set_pdf_invoices(pdf_invoices);
        std::string pdf_file_data{statement_pdf.generate(pdf_statement)};
        bool result{test::generate_statement_pdf(pdf_file_data, "3")};

        CHECK_EQUAL(false, result);
}
