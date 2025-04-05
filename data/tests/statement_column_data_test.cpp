/*******************************************************************************
 * Contents: statement column data unit tests
 * Author: Dawid Blom
 * Date: April 4, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <statement_column.h>
extern "C"
{

}


/**********************************TEST LIST*****************************************
 * 1) The user should have the ability to change the invoice number. (Done)
 * 2) The user should have the ability to change the date.
 * 3) The user should have the ability to change the order number.
 * 4) The user should have the ability to change the paid status.
 * 5) The user should have the ability to change the price.
 * 6) Ensure that the data maintains its integrity. (Done)
 * 7) Ensure that the data is thread safe.
 * 8) The invoice number may not exceed 8 characters in length. 0 < x <= 8 (Done)
 * 9) The date may not exceed 20 characters in length.
 * 10) The price may not exceed 14 characters in length.
 ************************************************************************************/
TEST_GROUP(statement_column_data_test)
{
        data::statement_column data{};
	void setup()
	{
	}

	void teardown()
	{
	}
};


// Red -> Green -> refactor

TEST(statement_column_data_test, change_invoice_number)
{
        std::string expected{"1234"};
        data.set_invoice_number(expected);
        std::string result{data.get_invoice_number()};

        STRCMP_EQUAL(result.c_str(), expected.c_str());
}

TEST(statement_column_data_test, successfully_maintain_data_integrity)
{
        std::string expected{"1234"};
        data.set_invoice_number(expected);

        CHECK(data.is_valid());
}

TEST(statement_column_data_test, unsuccessfully_maintain_data_integrity)
{
        std::string expected{""};
        data.set_invoice_number(expected);

        CHECK_EQUAL(false, data.is_valid());
}
