/*******************************************************************************
 * Contents: statement data unit tests
 * Author: Dawid Blom
 * Date: February, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


/*#include <client_data.h>*/
/*#include <invoice_data.h>*/
/*#include <business_data.h>*/
#include <statement_data.h>
#include <pdf_invoice_data.h>
#include <pdf_statement_data.h>
#include <mock_pdf_invoice_data.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) The data must be thread safe. (Done)
 * 2) The data must contain both the pdf_invoice_data and the statement data. (Done)
 * 3) Ensure that the user can copy the information. (Done)
 * 4) Ensure that the user can move the data somewhere. (Done)
 * 5) The user should be able to assign and retrieve the data. (Done)
 ******************************************************************************/
TEST_GROUP(pdf_statement_data_test)
{
	data::pdf_invoice pdf_invoice{};
        data::statement statement_data{};
	data::pdf_statement pdf_statement{};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(pdf_statement_data_test, assign_and_retrieve_bad_statement_data)
{
	pdf_statement.set_statement(statement_data);

        CHECK_EQUAL(false, pdf_statement.is_valid());
}

TEST(pdf_statement_data_test, assign_and_retrieve_good_statement_data)
{
        std::string invoice_number{"1234"};
        std::string date{"12/04/2025"};
        std::string order_number{"123 mdb"};
        std::string paid_status{"Yes"};
        std::string price{"R1234.00"};
        statement_data.set_invoice_number(invoice_number);
        statement_data.set_date(date);
        statement_data.set_order_number(order_number);
        statement_data.set_paid_status(paid_status);
        statement_data.set_price(price);
	pdf_statement.set_statement(statement_data);
	data::statement result{pdf_statement.get_statement()};

        CHECK_EQUAL(true, result.is_valid());
}

TEST(pdf_statement_data_test, assign_and_retrieve_bad_pdf_invoice_data)
{
	pdf_statement.set_pdf_invoice(pdf_invoice);

        CHECK_EQUAL(false, pdf_statement.is_valid());
}

TEST(pdf_statement_data_test, assign_and_retrieve_good_pdf_invoice_data)
{
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
	pdf_statement.set_pdf_invoice(pdf_invoice);
	data::pdf_invoice result{pdf_statement.get_pdf_invoice()};

        CHECK_EQUAL(true, result.is_valid());
}

TEST(pdf_statement_data_test, check_in_valid_data)
{
	pdf_statement.set_statement(statement_data);
	pdf_statement.set_pdf_invoice(pdf_invoice);

        CHECK_EQUAL(false, pdf_statement.is_valid());
}

TEST(pdf_statement_data_test, ensure_that_the_data_is_valid)
{
        std::string invoice_number{"1234"};
        std::string date{"12/04/2025"};
        std::string order_number{"123 mdb"};
        std::string paid_status{"Yes"};
        std::string price{"R1234.00"};
        statement_data.set_invoice_number(invoice_number);
        statement_data.set_date(date);
        statement_data.set_order_number(order_number);
        statement_data.set_paid_status(paid_status);
        statement_data.set_price(price);
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
	pdf_statement.set_statement(statement_data);
	pdf_statement.set_pdf_invoice(pdf_invoice);

        CHECK_EQUAL(true, pdf_statement.is_valid());
}

TEST(pdf_statement_data_test, ensure_copy_constructable)
{
        std::string invoice_number{"1234"};
        std::string date{"12/04/2025"};
        std::string order_number{"123 mdb"};
        std::string paid_status{"Yes"};
        std::string price{"R1234.00"};
        statement_data.set_invoice_number(invoice_number);
        statement_data.set_date(date);
        statement_data.set_order_number(order_number);
        statement_data.set_paid_status(paid_status);
        statement_data.set_price(price);
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
	pdf_statement.set_statement(statement_data);
	pdf_statement.set_pdf_invoice(pdf_invoice);
	data::pdf_statement result(pdf_statement);

        CHECK_EQUAL(true, result.is_valid());
}

TEST(pdf_statement_data_test, ensure_copy_assignmentable)
{
        std::string invoice_number{"1234"};
        std::string date{"12/04/2025"};
        std::string order_number{"123 mdb"};
        std::string paid_status{"Yes"};
        std::string price{"R1234.00"};
        statement_data.set_invoice_number(invoice_number);
        statement_data.set_date(date);
        statement_data.set_order_number(order_number);
        statement_data.set_paid_status(paid_status);
        statement_data.set_price(price);
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
	pdf_statement.set_statement(statement_data);
	pdf_statement.set_pdf_invoice(pdf_invoice);
	data::pdf_statement result;
	result = pdf_statement;

        CHECK_EQUAL(true, result.is_valid());
}

TEST(pdf_statement_data_test, ensure_move_constructable)
{
        std::string invoice_number{"1234"};
        std::string date{"12/04/2025"};
        std::string order_number{"123 mdb"};
        std::string paid_status{"Yes"};
        std::string price{"R1234.00"};
        statement_data.set_invoice_number(invoice_number);
        statement_data.set_date(date);
        statement_data.set_order_number(order_number);
        statement_data.set_paid_status(paid_status);
        statement_data.set_price(price);
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
	pdf_statement.set_statement(statement_data);
	pdf_statement.set_pdf_invoice(pdf_invoice);
	data::pdf_statement result{pdf_statement};

        CHECK_EQUAL(true, result.is_valid());
}

TEST(pdf_statement_data_test, ensure_move_assignmentable)
{
        std::string invoice_number{"1234"};
        std::string date{"12/04/2025"};
        std::string order_number{"123 mdb"};
        std::string paid_status{"Yes"};
        std::string price{"R1234.00"};
        statement_data.set_invoice_number(invoice_number);
        statement_data.set_date(date);
        statement_data.set_order_number(order_number);
        statement_data.set_paid_status(paid_status);
        statement_data.set_price(price);
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
	pdf_statement.set_statement(statement_data);
	pdf_statement.set_pdf_invoice(pdf_invoice);
	data::pdf_statement result;
	result = std::move(pdf_statement);

        CHECK_EQUAL(true, result.is_valid());
}
