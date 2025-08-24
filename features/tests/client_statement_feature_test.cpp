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
#include <pdf_statement_data.h>
#include <client_statement.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1)
 ******************************************************************************/
TEST_GROUP(client_statement_feature_test)
{
	controller::client_statement statement{};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(client_statement_feature_test, load_data_from_db)
{
	std::vector<data::pdf_statement> result{};
	for (const std::any& data : statement.load("test_business"))
	{
		data::pdf_statement pdf_statement{std::any_cast<data::pdf_statement>(data)};
		result.emplace_back(pdf_statement);
	}

	CHECK_EQUAL(false, result.empty());
}
