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
#include <serialize_admin.h>

extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Ensure the ability to convert data to SQL arguments.
 * 2) Ensure the ability to convert SQL data to admin struct data.
 ******************************************************************************/
TEST_GROUP(serialize_admin_test)
{
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(serialize_admin_test, convert_data_to_sql_arguments)
{
	utility::serialize_admin serialize_admin{};
	data::admin admin_data{test::generate_business_data()};
	storage::database::sql_parameters sql_parameters{serialize_admin.package_data(admin_data)};

	CHECK_EQUAL(false, sql_parameters.empty());
}
