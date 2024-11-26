/*******************************************************************************
 * Contents: Sql unit tests
 * Author: Dawid Blom
 * Date: November 18, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <sql.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1)
 ******************************************************************************/
TEST_GROUP(sql_test)
{
        storage::sql sql{};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(sql_test, insert)
{
        CHECK_EQUAL(true, sql.insert());
}

TEST(sql_test, update)
{
        CHECK_EQUAL(true, sql.update());
}
