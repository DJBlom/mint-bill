/*******************************************************************************
 * Contents: statement data unit tests
 * Author: Dawid Blom
 * Date: February, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <iostream>
#include <vector>
#include <statement_data.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1)
 ******************************************************************************/
TEST_GROUP(statement_data_test)
{
        data::statement statement_data;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(statement_data_test, handle_the_business_name)
{
}
