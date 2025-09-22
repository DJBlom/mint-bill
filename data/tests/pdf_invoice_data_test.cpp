/*******************************************************************************
 * Contents: PDF invoice data unit tests
 * Author: Dawid Blom
 * Date: December 23, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <pdf_invoice_data.h>
#include <mock_pdf_invoice_data.h>
extern "C"
{

}



/**********************************TEST LIST************************************
 * 1) Assign client data. (Done)
 * 2) Retrieve client data. (Done)
 * 3) Assign invoice data. (Done)
 * 4) Retrieve invoice data. (Done)
 * 5) Assign admin data. (Done)
 * 6) Retrieve admin data. (Done)
 * 7) Ensure thread safety. (Done)
 * 8) Ensure data copy and moveability. (Done)
 ******************************************************************************/
TEST_GROUP(pdf_invoice_data_test)
{
        data::pdf_invoice pdf_invoice;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(pdf_invoice_data_test, assign_and_retrieve_client_data)
{
        pdf_invoice.set_client(retrieve_client_data());
        data::client result{pdf_invoice.get_client()};

	CHECK_EQUAL(true, result.is_valid());
}

TEST(pdf_invoice_data_test, assign_and_retrieve_bad_client_data)
{
        pdf_invoice.set_client(retrieve_bad_client_data());
        data::client result{pdf_invoice.get_client()};

	CHECK_EQUAL(false, result.is_valid());
}

TEST(pdf_invoice_data_test, assign_and_retrieve_invoice_data)
{
        pdf_invoice.set_invoice(retrieve_invoice_data());
        data::invoice result{pdf_invoice.get_invoice()};

	CHECK_EQUAL(true, result.is_valid());
}

TEST(pdf_invoice_data_test, assign_and_retrieve_bad_invoice_data)
{
        pdf_invoice.set_invoice(retrieve_bad_invoice_data());
        data::invoice result{pdf_invoice.get_invoice()};

	CHECK_EQUAL(false, result.is_valid());
}

TEST(pdf_invoice_data_test, assign_and_retrieve_business_data)
{
        pdf_invoice.set_business(retrieve_business_data());
        data::admin result{pdf_invoice.get_business()};

	CHECK_EQUAL(true, result.is_valid());
}

TEST(pdf_invoice_data_test, assign_and_retrieve_bad_business_data)
{
        pdf_invoice.set_business(retrieve_bad_business_data());
        data::admin result{pdf_invoice.get_business()};

	CHECK_EQUAL(false, result.is_valid());
}

TEST(pdf_invoice_data_test, copy_assign_data)
{
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
        data::admin result{pdf_invoice.get_business()};
        data::pdf_invoice tmp;
        tmp = pdf_invoice;

	CHECK_EQUAL(true, tmp.is_valid());
}

TEST(pdf_invoice_data_test, verify_correct_data)
{
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());

	CHECK_EQUAL(true, pdf_invoice.is_valid());
}

TEST(pdf_invoice_data_test, verify_incorrect_data)
{
        pdf_invoice.set_client(retrieve_bad_client_data());
        pdf_invoice.set_invoice(retrieve_bad_invoice_data());
        pdf_invoice.set_business(retrieve_bad_business_data());

	CHECK_EQUAL(false, pdf_invoice.is_valid());
}

TEST(pdf_invoice_data_test, move_assign_data)
{
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
        data::admin result{pdf_invoice.get_business()};
        data::pdf_invoice tmp;
        tmp = std::move(pdf_invoice);

	CHECK_EQUAL(true, tmp.is_valid());
}

TEST(pdf_invoice_data_test, copy_construct_data)
{
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
        data::admin result{pdf_invoice.get_business()};
        data::pdf_invoice tmp{pdf_invoice};

	CHECK_EQUAL(true, tmp.is_valid());
}

TEST(pdf_invoice_data_test, move_construct_data)
{
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
        data::admin result{pdf_invoice.get_business()};
        data::pdf_invoice tmp{std::move(pdf_invoice)};

	CHECK_EQUAL(true, tmp.is_valid());
}
