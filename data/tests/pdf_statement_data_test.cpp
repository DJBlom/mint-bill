/*******************************************************************************
 * Contents: statement data unit tests
 * Author: Dawid Blom
 * Date: February, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <pdf_invoice_data.h>
#include <pdf_statement_data.h>
#include <mock_pdf_invoice_data.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) The data must be thread safe. (Done)
 * 2) The data must contain both the pdf_invoice_data and the statement data.
 *    The ratio is a many pdf_invoice_data for one statment_data. (Done)
 * 3) Ensure that the user can copy the information. (Done)
 * 4) Ensure that the user can move the data somewhere. (Done)
 * 5) The user should be able to assign and retrieve the data. (Done)
 * 6) Asisgn and retrieve the statement price total.
 ******************************************************************************/
TEST_GROUP(pdf_statement_data_test)
{
	std::string statement_number{"# 1"};
	std::string statement_date{"2025-05-31"};
	std::string statement_total{"R 1234.00"};
	data::pdf_statement pdf_statement{};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(pdf_statement_data_test, assign_and_retrieve_statement_number)
{
	pdf_statement.set_number(statement_number);

        CHECK_EQUAL(statement_number, pdf_statement.get_number());
}

TEST(pdf_statement_data_test, assign_and_retrieve_statement_date)
{
	pdf_statement.set_date(statement_date);

        CHECK_EQUAL(statement_date, pdf_statement.get_date());
}

TEST(pdf_statement_data_test, assign_and_retrieve_price_total)
{
	pdf_statement.set_total(statement_total);

        CHECK_EQUAL(statement_total, pdf_statement.get_total());
}

TEST(pdf_statement_data_test, assign_and_retrieve_bad_pdf_invoice_data)
{
	std::vector<data::pdf_invoice> pdf_invoices{};
	pdf_statement.set_pdf_invoices(pdf_invoices);

        CHECK_EQUAL(false, pdf_statement.is_valid());
}

TEST(pdf_statement_data_test, assign_and_retrieve_pdf_invoice_data)
{
	data::pdf_invoice pdf_invoice{};
	std::vector<data::pdf_invoice> pdf_invoices{};
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
	pdf_invoices.push_back(pdf_invoice);
	pdf_statement.set_pdf_invoices(pdf_invoices);
	std::vector<data::pdf_invoice> result{pdf_statement.get_pdf_invoices()};

        CHECK_EQUAL(true, result[0].is_valid());
}

TEST(pdf_statement_data_test, check_invalid_data)
{
	std::vector<data::pdf_invoice> pdf_invoices{};
	pdf_statement.set_number("");
	pdf_statement.set_date("");
	pdf_statement.set_total(statement_total);
	pdf_statement.set_pdf_invoices(pdf_invoices);

        CHECK_EQUAL(false, pdf_statement.is_valid());
}

TEST(pdf_statement_data_test, ensure_that_the_data_is_valid)
{
	data::pdf_invoice pdf_invoice{};
	std::vector<data::pdf_invoice> pdf_invoices{};
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
	pdf_invoices.push_back(pdf_invoice);
	pdf_statement.set_number(statement_number);
	pdf_statement.set_date(statement_date);
	pdf_statement.set_total(statement_total);
	pdf_statement.set_pdf_invoices(pdf_invoices);

        CHECK_EQUAL(true, pdf_statement.is_valid());
}

TEST(pdf_statement_data_test, ensure_a_ratio_of_many_to_one)
{
	std::vector<data::pdf_invoice> pdf_invoices{};
	for (int i = 0; i < 10; ++i)
	{
		data::pdf_invoice pdf_invoice{};
		pdf_invoice.set_client(retrieve_client_data());
		pdf_invoice.set_invoice(retrieve_invoice_data());
		pdf_invoice.set_business(retrieve_business_data());
		pdf_invoices.push_back(pdf_invoice);
	}
	pdf_statement.set_number(statement_number);
	pdf_statement.set_date(statement_date);
	pdf_statement.set_total(statement_total);
	pdf_statement.set_pdf_invoices(pdf_invoices);

        CHECK_EQUAL(true, pdf_statement.is_valid());
}

TEST(pdf_statement_data_test, ensure_copy_constructable)
{
	data::pdf_invoice pdf_invoice{};
	std::vector<data::pdf_invoice> pdf_invoices{};
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
	pdf_invoices.push_back(pdf_invoice);
	pdf_statement.set_number(statement_number);
	pdf_statement.set_date(statement_date);
	pdf_statement.set_total(statement_total);
	pdf_statement.set_pdf_invoices(pdf_invoices);
	data::pdf_statement result(pdf_statement);

        CHECK_EQUAL(true, result.is_valid());
}

TEST(pdf_statement_data_test, ensure_copy_assignmentable)
{
	data::pdf_invoice pdf_invoice{};
	std::vector<data::pdf_invoice> pdf_invoices{};
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
	pdf_invoices.push_back(pdf_invoice);
	pdf_statement.set_number(statement_number);
	pdf_statement.set_date(statement_date);
	pdf_statement.set_total(statement_total);
	pdf_statement.set_pdf_invoices(pdf_invoices);
	data::pdf_statement result;
	result = pdf_statement;

        CHECK_EQUAL(true, result.is_valid());
}

TEST(pdf_statement_data_test, ensure_move_constructable)
{
	data::pdf_invoice pdf_invoice{};
	std::vector<data::pdf_invoice> pdf_invoices{};
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
	pdf_invoices.push_back(pdf_invoice);
	pdf_statement.set_number(statement_number);
	pdf_statement.set_date(statement_date);
	pdf_statement.set_total(statement_total);
	pdf_statement.set_pdf_invoices(pdf_invoices);
	data::pdf_statement result{pdf_statement};

        CHECK_EQUAL(true, result.is_valid());
}

TEST(pdf_statement_data_test, ensure_move_assignmentable)
{
	data::pdf_invoice pdf_invoice{};
	std::vector<data::pdf_invoice> pdf_invoices{};
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
	pdf_invoices.push_back(pdf_invoice);
	pdf_statement.set_number(statement_number);
	pdf_statement.set_date(statement_date);
	pdf_statement.set_total(statement_total);
	pdf_statement.set_pdf_invoices(pdf_invoices);
	data::pdf_statement result;
	result = std::move(pdf_statement);

        CHECK_EQUAL(true, result.is_valid());
}
