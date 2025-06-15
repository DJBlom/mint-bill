/*******************************************************************************
 * Contents: Statement feature unit tests
 * Author: Dawid Blom
 * Date: June 11, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <vector>
#include <iostream>
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
	std::vector<data::statement> result{};
	std::vector<std::any> tmp = statement.load("test_business");
	for (const std::any& stm : tmp)
	{
		result.push_back(std::any_cast<data::statement>(stm));
	}

	CHECK_EQUAL(false, result.empty());
}
