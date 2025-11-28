/*******************************************************************************
 * Contents: Invoice model unit tests
 * Author: Dawid Blom
 * Date: December 9, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <sqlite.h>
#include <generate_pdf.h>
#include <invoice_model.h>
#include <admin_serialize.h>
#include <client_serialize.h>
#include <invoice_serialize.h>
#include <business_serialize.h>
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
	storage::database::sqlite database{db_file, db_password};
	void setup()
	{
		data::client client_data{test::generate_client_data()};
		data::business client_business_data{client_data};
		data::admin admin_data{test::generate_business_data()};
		data::business admin_business_data{admin_data};
		serialize::admin admin_serialize{};
		serialize::client client_serialize{};
		serialize::invoice invoice_serialize{};
		serialize::business business_serialize{};
		storage::database::sql_parameters admin_sql_parameters{admin_serialize.package_data(admin_data)};
		storage::database::sql_parameters admin_business_sql_parameters{business_serialize.package_data(admin_business_data)};
		storage::database::sql_parameters client_business_sql_parameters{business_serialize.package_data(client_business_data)};
		storage::database::sql_parameters client_sql_parameters{client_serialize.package_data(client_data)};
		(void)database.select(sql::query::business_details_usert, admin_business_sql_parameters);
		(void)database.select(sql::query::admin_usert, admin_sql_parameters);
		(void)database.select(sql::query::business_details_usert, client_business_sql_parameters);
		(void)database.select(sql::query::client_usert, client_sql_parameters);
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

        CHECK_EQUAL(true, invoice_model.save(invoice_data));
}

TEST(invoice_model_test, load_data_from_database_unsuccessfully)
{
	for (const std::any pdf_invoice_data : invoice_model.load(""))
	{
		data::pdf_invoice data{std::any_cast<data::pdf_invoice> (pdf_invoice_data)};

		CHECK_EQUAL(false, data.is_valid());
	}
}

TEST(invoice_model_test, load_data_from_database_successfully)
{
        data::invoice invoice_data{test::generate_invoice_data("invoice model machining")};
	for (const std::any pdf_invoice_data : invoice_model.load(invoice_data.get_name()))
	{
		data::pdf_invoice data{std::any_cast<data::pdf_invoice> (pdf_invoice_data)};
		data::admin admin_data{data.get_business()};
		data::client client_data{data.get_client()};
		data::invoice invoice_data{data.get_invoice()};

		CHECK_EQUAL(true, admin_data.is_valid());
		CHECK_EQUAL(true, client_data.is_valid());
		CHECK_EQUAL(true, invoice_data.is_valid());
		CHECK_EQUAL(true, data.is_valid());
	}
}
