/*******************************************************************************
 * Contents: PDF invoice data unit tests
 * Author: Dawid Blom
 * Date: December 23, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <client_data.h>
#include <pdf_invoice_data.h>
extern "C"
{

}


static data::client retrieve_client_data();
static data::client retrieve_bad_client_data();
static data::invoice retrieve_invoice_data();
static data::invoice retrieve_bad_invoice_data();
static data::business retrieve_business_data();
static data::business retrieve_bad_business_data();

/**********************************TEST LIST************************************
 * 1) Assign client data. (Done)
 * 2) Retrieve client data. (Done)
 * 3) Assign invoice data. (Done)
 * 4) Retrieve invoice data. (Done)
 * 5) Assign business data. (Done)
 * 6) Retrieve business data. (Done)
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
        data::business result{pdf_invoice.get_business()};

	CHECK_EQUAL(true, result.is_valid());
}

TEST(pdf_invoice_data_test, assign_and_retrieve_bad_business_data)
{
        pdf_invoice.set_business(retrieve_bad_business_data());
        data::business result{pdf_invoice.get_business()};

	CHECK_EQUAL(false, result.is_valid());
}

TEST(pdf_invoice_data_test, copy_assign_data)
{
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
        data::business result{pdf_invoice.get_business()};
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
        data::business result{pdf_invoice.get_business()};
        data::pdf_invoice tmp;
        tmp = std::move(pdf_invoice);

	CHECK_EQUAL(true, tmp.is_valid());
}

TEST(pdf_invoice_data_test, copy_construct_data)
{
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
        data::business result{pdf_invoice.get_business()};
        data::pdf_invoice tmp{pdf_invoice};

	CHECK_EQUAL(true, tmp.is_valid());
}

TEST(pdf_invoice_data_test, move_construct_data)
{
        pdf_invoice.set_client(retrieve_client_data());
        pdf_invoice.set_invoice(retrieve_invoice_data());
        pdf_invoice.set_business(retrieve_business_data());
        data::business result{pdf_invoice.get_business()};
        data::pdf_invoice tmp{std::move(pdf_invoice)};

	CHECK_EQUAL(true, tmp.is_valid());
}

static data::client retrieve_client_data()
{
        data::client expected;
        expected.set_business_name("TME");
        expected.set_business_address("Geelsterd 8");
        expected.set_business_area_code("543543");
        expected.set_business_town_name("George");
        expected.set_cellphone_number("0832315944");
        expected.set_email("odn@gmail.com");
        expected.set_vat_number("3241324321413");
        expected.set_statement_schedule("4,4");

        return expected;
}

static data::client retrieve_bad_client_data()
{
        data::client expected;
        expected.set_business_name("");
        expected.set_business_address("Geelsterd 8");
        expected.set_business_area_code("543543");
        expected.set_business_town_name("George");
        expected.set_cellphone_number("0832315944");
        expected.set_email("odn@gmail.com");
        expected.set_vat_number("3241324321413");
        expected.set_statement_schedule("4,4");

        return expected;
}

static data::invoice retrieve_invoice_data()
{
        const int size{50};
        std::vector<data::column> vec{};
        for (unsigned int i = 0; i < size; ++i)
        {
                data::column expected{};
                expected.set_quantity(i);
                expected.set_description("machining");
                expected.set_amount(55 + i + .0);
                vec.push_back(expected);
        }
        data::invoice expected;
        expected.set_business_name("Test Business");
        expected.set_invoice_number("134");
        expected.set_invoice_date("2023-09-04");
        expected.set_job_card_number("24/md");
        expected.set_order_number("order 123");
        expected.set_description_total("1234.00");
        expected.set_material_total("1234.00");
        expected.set_grand_total("1234.00");
        expected.set_material_column(vec);
        expected.set_description_column(vec);

        return expected;
}

static data::invoice retrieve_bad_invoice_data()
{
        const int size{50};
        std::vector<data::column> vec{};
        for (unsigned int i = 0; i < size; ++i)
        {
                data::column expected{};
                expected.set_quantity(i);
                expected.set_description("machining");
                expected.set_amount(55 + i + .0);
                vec.push_back(expected);
        }
        data::invoice expected;
        expected.set_business_name("");
        expected.set_invoice_number("134");
        expected.set_invoice_date("2023-09-04");
        expected.set_job_card_number("24/md");
        expected.set_order_number("order 123");
        expected.set_description_total("1234.00");
        expected.set_material_total("1234.00");
        expected.set_grand_total("1234.00");
        expected.set_material_column(vec);
        expected.set_description_column(vec);

        return expected;
}

static data::business retrieve_business_data()
{
        data::business expected;
        expected.set_name("tme");
        expected.set_address("geelsterd 8");
        expected.set_area_code("5432");
        expected.set_town("george");
        expected.set_cellphone("0832315944");
        expected.set_email("odn@gmail.com");
        expected.set_bank("Standard Bank");
        expected.set_branch_code("043232");
        expected.set_account_number("0932443824");
        expected.set_client_message("Thank you for your support");
        expected.set_password("");

        return expected;
}

static data::business retrieve_bad_business_data()
{
        data::business expected;
        expected.set_name("");
        expected.set_address("geelsterd 8");
        expected.set_area_code("5432");
        expected.set_town("george");
        expected.set_cellphone("0832315944");
        expected.set_email("odn@gmail.com");
        expected.set_bank("Standard Bank");
        expected.set_branch_code("043232");
        expected.set_account_number("0932443824");
        expected.set_client_message("Thank you for your support");
        expected.set_password("");

        return expected;
}
