/*******************************************************************************
 * Contents: Invoice model unit tests
 * Author: Dawid Blom
 * Date: December 9, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <generate_pdf.h>
#include <invoice_model.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Load the data from a database.
 * 2) Save the data into a database.
 ******************************************************************************/
TEST_GROUP(invoice_model_test)
{
	const std::string db_file{"../storage/tests/model_test.db"};
	const std::string db_password{"123456789"};
        model::invoice invoice_model{db_file, db_password};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(invoice_model_test, save_data_to_database_failure)
{
        data::invoice invoice_data{};

        CHECK_EQUAL(false, invoice_model.save(invoice_data));
}

TEST(invoice_model_test, save_data_to_database_successfully)
{
        data::invoice invoice_data{test::generate_invoice_data("invoice model machining")};

        CHECK_EQUAL(true, invoice_model.save(invoice_data));
}

TEST(invoice_model_test, save_second_invoice_to_database_successfully)
{
        data::invoice invoice_data{test::generate_invoice_data("invoice model testing")};
	invoice_data.set_order_number("13245kddf");

        CHECK_EQUAL(true, invoice_model.save(invoice_data));
}

TEST(invoice_model_test, load_data_from_database_unsuccessfully)
{
	for (const std::any pdf_invoice_data : invoice_model.load(""))
	{
		data::pdf_invoice data = std::any_cast<data::pdf_invoice> (pdf_invoice_data);

		CHECK_EQUAL(false, data.is_valid());
	}
}

TEST(invoice_model_test, load_data_from_database_successfully)
{
        data::invoice invoice_data{test::generate_invoice_data("invoice model machining")};
	for (const std::any pdf_invoice_data : invoice_model.load(invoice_data.get_business_name()))
	{
		data::pdf_invoice data = std::any_cast<data::pdf_invoice> (pdf_invoice_data);

		CHECK_EQUAL(true, data.is_valid());
	}
}
