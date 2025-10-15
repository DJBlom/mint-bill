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
