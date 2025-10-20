/*******************************************************************************
 * Contents: Serialize unit tests
 * Author: Dawid Blom
 * Date: December 9, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <sqlite.h>
#include <generate_pdf.h>
#include <business_serialize.h>
#include <admin_serialize.h>
#include <client_serialize.h>
#include <invoice_serialize.h>


extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Ensure the ability to convert data to SQL arguments. (Done)
 * 2) Ensure the ability to convert SQL data to admin struct data. (Done)
 ******************************************************************************/
TEST_GROUP(business_serialize_test)
{
	const std::string db_file{"../storage/tests/model_test.db"};
	const std::string db_password{"123456789"};
	storage::database::sqlite database{db_file, db_password};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(business_serialize_test, convert_data_to_sql_arguments_unsuccessfully)
{
	data::business business_data{};
	serialize::business business_serialize{};
	storage::database::sql_parameters sql_parameters{business_serialize.package_data(business_data)};

	CHECK_EQUAL(true, sql_parameters.empty());
}

TEST(business_serialize_test, convert_data_to_sql_arguments_successfully)
{
	data::admin admin_data{test::generate_business_data()};
	data::business business_data{};
	business_data.set_name(admin_data.get_name());
	business_data.set_address(admin_data.get_address());
	business_data.set_area_code(admin_data.get_area_code());
	business_data.set_town(admin_data.get_town());
	business_data.set_cellphone(admin_data.get_cellphone());
	business_data.set_email(admin_data.get_email());
	serialize::business business_serialize{};
	storage::database::sql_parameters sql_parameters{business_serialize.package_data(business_data)};

	CHECK_EQUAL(false, sql_parameters.empty());
}

TEST(business_serialize_test, convert_sql_to_business_data_unsuccessfully)
{
	serialize::business business_serialize{};
	std::vector<storage::database::param_values> params = {};
	data::business business_data{
		std::any_cast<data::business>(
			business_serialize.extract_data(
				database.select(sql::query::business_details_select, params)
			)
		)
	};

	CHECK_EQUAL(false, business_data.is_valid());
}

TEST(business_serialize_test, convert_sql_business_data_successfully)
{
	serialize::business business_serialize{};
	data::admin tmp_admin_data{test::generate_business_data()};
	std::string business_name{tmp_admin_data.get_name()};
	std::vector<storage::database::param_values> params = {business_name};
	data::business business_data{
		std::any_cast<data::business>(
			business_serialize.extract_data(
				database.select(sql::query::business_details_select, params)
			)
		)
	};

	CHECK_EQUAL(true, business_data.is_valid());
}






/**********************************TEST LIST************************************
 * 1) Ensure the ability to convert data to SQL arguments. (Done)
 * 2) Ensure the ability to convert SQL data to admin struct data. (Done)
 ******************************************************************************/
TEST_GROUP(admin_serialize_test)
{
	const std::string db_file{"../storage/tests/model_test.db"};
	const std::string db_password{"123456789"};
	storage::database::sqlite database{db_file, db_password};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(admin_serialize_test, convert_data_to_sql_arguments_unsuccessfully)
{
	data::admin admin_data{};
	serialize::admin admin_serialize{};
	storage::database::sql_parameters sql_parameters{admin_serialize.package_data(admin_data)};

	CHECK_EQUAL(true, sql_parameters.empty());
}

TEST(admin_serialize_test, convert_data_to_sql_arguments_successfully)
{
	serialize::admin admin_serialize{};
	data::admin admin_data{test::generate_business_data()};
	storage::database::sql_parameters sql_parameters{admin_serialize.package_data(admin_data)};

	CHECK_EQUAL(false, sql_parameters.empty());
}

TEST(admin_serialize_test, convert_sql_to_admin_data_unsuccessfully)
{
	serialize::admin admin_serialize{};
	std::vector<storage::database::param_values> params = {};
	data::admin admin_data{
		std::any_cast<data::admin>(
			admin_serialize.extract_data(
				database.select(sql::query::admin_select, params)
			)
		)
	};

	CHECK_EQUAL(false, admin_data.is_valid());
}

TEST(admin_serialize_test, convert_sql_admin_data_successfully)
{
	serialize::admin admin_serialize{};
	data::admin tmp_admin_data{test::generate_business_data()};
	std::string business_name{tmp_admin_data.get_name()};
	std::vector<storage::database::param_values> params = {business_name};
	data::admin admin_data{
		std::any_cast<data::admin>(
			admin_serialize.extract_data(
				database.select(sql::query::admin_select, params)
			)
		)
	};

	CHECK_EQUAL(true, admin_data.is_valid());
}





/**********************************TEST LIST************************************
 * 1) Ensure the ability to convert data to SQL arguments. (Done)
 * 2) Ensure the ability to convert SQL data to admin struct data. (Done)
 ******************************************************************************/
TEST_GROUP(client_serialize_test)
{
	const std::string db_file{"../storage/tests/model_test.db"};
	const std::string db_password{"123456789"};
	storage::database::sqlite database{db_file, db_password};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(client_serialize_test, convert_data_to_sql_arguments_unsuccessfully)
{
	data::client client_data{};
	serialize::client client_serialize{};
	storage::database::sql_parameters sql_parameters{client_serialize.package_data(client_data)};

	CHECK_EQUAL(true, sql_parameters.empty());
}

TEST(client_serialize_test, convert_data_to_sql_arguments_successfully)
{
	data::client client_data{test::generate_client_data()};
	serialize::client client_serialize{};
	storage::database::sql_parameters sql_parameters{client_serialize.package_data(client_data)};

	CHECK_EQUAL(false, sql_parameters.empty());
}

TEST(client_serialize_test, convert_sql_to_business_data_unsuccessfully)
{
	serialize::client client_serialize{};
	std::vector<storage::database::param_values> params = {};
	data::client client_data{
		std::any_cast<data::client>(
			client_serialize.extract_data(
				database.select(sql::query::client_select, params)
			)
		)
	};

	CHECK_EQUAL(false, client_data.is_valid());
}

TEST(client_serialize_test, convert_sql_business_data_successfully)
{
	serialize::client client_serialize{};
	data::client tmp_client_data{test::generate_client_data()};
	std::vector<storage::database::param_values> params = {tmp_client_data.get_name()};
	data::client client_data{
		std::any_cast<data::client>(
			client_serialize.extract_data(
				database.select(sql::query::client_select, params)
			)
		)
	};

	CHECK_EQUAL(true, client_data.is_valid());
}





/**********************************TEST LIST************************************
 * 1) Ensure the ability to convert data to SQL arguments. (Done)
 * 2) Ensure the ability to convert SQL data to admin struct data. (Done)
 ******************************************************************************/
TEST_GROUP(column_serialize_test)
{
	const std::string db_file{"../storage/tests/model_test.db"};
	const std::string db_password{"123456789"};
	storage::database::sqlite database{db_file, db_password};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(column_serialize_test, convert_data_to_sql_arguments_invalid_column_data)
{
	data::invoice invoice_data{test::generate_invoice_data("Serialize test")};
	data::column column_data{};
	serialize::column column_serialize{};
	storage::database::sql_parameters sql_parameters{column_serialize.package_data(column_data, invoice_data)};

	CHECK_EQUAL(true, sql_parameters.empty());
}

TEST(column_serialize_test, convert_data_to_sql_arguments_invalid_invoice_data)
{
	data::column column_data{};
	data::invoice invoice_data{test::generate_invoice_data("Serialize test")};
	serialize::column column_serialize{};
	storage::database::sql_parameters sql_parameters{column_serialize.package_data(column_data, invoice_data)};

	CHECK_EQUAL(true, sql_parameters.empty());
}

TEST(column_serialize_test, convert_data_to_sql_arguments_successfully)
{
	serialize::column column_serialize{};
	data::invoice invoice_data{test::generate_invoice_data("Serialization test")};
	for (const data::column& column_data : invoice_data.get_description_column())
	{
		storage::database::sql_parameters sql_parameters{column_serialize.package_data(column_data, invoice_data)};

		CHECK_EQUAL(false, sql_parameters.empty());
	}
}

TEST(column_serialize_test, convert_sql_to_business_data_unsuccessfully)
{
	serialize::column column_serialize{};
	std::vector<storage::database::param_values> params = {};

	for (const data::column& column_data : column_serialize.extract_data(
				database.select(sql::query::description_labor_select, params)
			))
	{
		CHECK_EQUAL(false, column_data.is_valid());
	}
}

TEST(column_serialize_test, convert_sql_to_business_data_successfully)
{
	serialize::column column_serialize{};
	data::invoice invoice_data{test::generate_invoice_data("Serialization test")};
	std::vector<storage::database::param_values> params = {static_cast<long long> (std::stoi(invoice_data.get_invoice_number()))};

	for (const data::column& column_data : column_serialize.extract_data(
				database.select(sql::query::description_labor_select, params)))
	{
		CHECK_EQUAL(true, column_data.is_valid());
	}
}




/**********************************TEST LIST************************************
 * 1) Ensure the ability to convert data to SQL arguments. (Done)
 * 2) Ensure the ability to convert SQL data to admin struct data. (Done)
 ******************************************************************************/
TEST_GROUP(invoice_serialize_test)
{
	const std::string db_file{"../storage/tests/model_test.db"};
	const std::string db_password{"123456789"};
	storage::database::sqlite database{db_file, db_password};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(invoice_serialize_test, convert_data_to_sql_arguments_unsuccessfully)
{
	data::invoice invoice_data{};
	serialize::invoice invoice_serialize{};
	storage::database::sql_parameters sql_parameters{invoice_serialize.package_data(invoice_data)};

	CHECK_EQUAL(true, sql_parameters.empty());
}

TEST(invoice_serialize_test, convert_data_to_sql_arguments_successfully)
{
	data::invoice invoice_data{test::generate_invoice_data("Serialization test")};
	serialize::invoice invoice_serialize{};
	storage::database::sql_parameters sql_parameters{invoice_serialize.package_data(invoice_data)};

	CHECK_EQUAL(false, sql_parameters.empty());
}

TEST(invoice_serialize_test, convert_sql_to_business_data_unsuccessfully)
{
	serialize::invoice invoice_serialize{};
	std::vector<storage::database::param_values> params = {};

	for (const std::any& data : invoice_serialize.extract_data(
			database.select(sql::query::invoice_select, params)))
	{
		data::invoice invoice_data{std::any_cast<data::invoice> (data)};

		CHECK_EQUAL(false, invoice_data.is_valid());
	}
}

TEST(invoice_serialize_test, convert_sql_to_business_data_successfully)
{
	serialize::column column_serialize{};
	serialize::invoice invoice_serialize{};
	data::invoice tmp_invoice_data{test::generate_invoice_data("Serialization test")};
	storage::database::sql_parameters invoice_params = {tmp_invoice_data.get_business_name()};
	for (const std::any& data : invoice_serialize.extract_data(
			database.select(sql::query::invoice_select, invoice_params)))
	{
		data::invoice invoice_data{std::any_cast<data::invoice> (data)};
		storage::database::sql_parameters column_params = {std::stoi(invoice_data.get_invoice_number())};
		std::vector<data::column> description_column_data{column_serialize.extract_data(
					database.select(sql::query::description_labor_select, column_params)
				)};
		std::vector<data::column> material_column_data{column_serialize.extract_data(
					database.select(sql::query::material_labor_select, column_params)
				)};
		invoice_data.set_description_column(description_column_data);
		invoice_data.set_material_column(material_column_data);

		CHECK_EQUAL(true, invoice_data.is_valid());
	}
}
