/*******************************************************************************
 * Contents: Sql unit tests
 * Author: Dawid Blom
 * Date: November 18, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


extern "C"
{

}


/**********************************TEST LIST************************************
 * 1)
 ******************************************************************************/
TEST_GROUP(sql_test)
{
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(sql_test, pass)
{
	CHECK_EQUAL(true, true);
}
