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
#include <statement_column.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) The data must be thread safe.
 * 2) The data must contain both the statement column data, the invoice data,
 *    the client data, and the business data.
 * 3) Ensure that the user can copy the information.
 * 4) Ensure that the user can move the data somewhere.
 * 5) The user should be able to assign and retrieve the data.
 ******************************************************************************/
TEST_GROUP(statement_data_test)
{
        data::statement_column columns{};
        data::statement statement_data;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(statement_data_test, assign_and_retrieve_the_statement_column_data)
{
        std::string invoice_number{"1234"};
        std::string date{"12/04/2025"};
        std::string order_number{"123 mdb"};
        std::string paid_status{"Yes"};
        std::string price{"R1234.00"};
        columns.set_invoice_number(invoice_number);
        columns.set_date(date);
        columns.set_order_number(order_number);
        columns.set_paid_status(paid_status);
        columns.set_price(price);
	statement_data.set_columns(columns);
	data::statement_column result{statement_data.get_columns()};

        CHECK_EQUAL(true, result.is_valid());
}
