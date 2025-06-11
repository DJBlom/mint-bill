/*******************************************************************************
 * Contents: Statement feature unit tests
 * Author: Dawid Blom
 * Date: June 11, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <sql.h>
#include <statement_data.h>
#include <client_statement.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1)
 ******************************************************************************/
TEST_GROUP(client_statement_feature_test)
{
        storage::sql db{};
	feature::client_statement statement{};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(client_statement_feature_test, load_data_from_db)
{
	data::statement result{std::any_cast<data::statement>(statement.load("test_business", db))};

	CHECK_EQUAL(true, result.is_valid());
}
