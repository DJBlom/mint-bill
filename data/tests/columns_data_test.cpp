/*******************************************************************************
 * Contents: invoice data unit tests
 * Author: Dawid Blom
 * Date: December 4, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <climits>
#include <column_data.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Assign the quantity column value. (Done)
 * 2) Retrieve the quantity column value. (Done)
 * 3) Assign the description column value. (Done)
 * 4) Retrieve the description column value. (Done)
 * 5) Assign the amount column value. (Done)
 * 6) Retrieve the amount column value. (Done)
 * 7) Verify the validity of the data. (Done)
 * 8) Ensure data is copyable. (Done)
 * 9) Ensure data is moveable. (Done)
 * 10) Quantity column max value is (0 < length < 9).
 * 11) Description column max lenth is (0 < length < 200).
 * 12) Amount column max length is (0 < length < 15)
 * 13) Ensure thread safety of the data. (Done)
 ******************************************************************************/
TEST_GROUP(invoice_column_test)
{
        data::column column;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(invoice_column_test, handle_assigning_and_retrieving_quantity_column)
{
        unsigned int expected{12};
        column.set_quantity(expected);

        CHECK_EQUAL(expected, column.get_quantity());
}

TEST(invoice_column_test, handle_assigning_and_retrieving_description_column)
{
        std::string expected{"Machinging"};
        column.set_description(expected);

        CHECK_EQUAL(expected, column.get_description());
}

TEST(invoice_column_test, handle_assigning_and_retrieving_amount_column)
{
        double expected{5558.99};
        column.set_amount(expected);

        CHECK_EQUAL(expected, column.get_amount());
}

TEST(invoice_column_test, handle_data_verification)
{
        bool expected{true};
        unsigned int quantity{12};
        std::string description{"Machinging"};
        double amount{5558.99};
        column.set_quantity(quantity);
        column.set_description(description);
        column.set_amount(amount);

        CHECK_EQUAL(expected, column.is_valid());
}

TEST(invoice_column_test, handle_data_copy_assignement)
{
        bool expected{true};
        unsigned int quantity{12};
        std::string description{"Machinging"};
        double amount{5558.99};
        column.set_quantity(quantity);
        column.set_description(description);
        column.set_amount(amount);
        data::column tmp;
        tmp = column;

        CHECK_EQUAL(expected, tmp.is_valid());
}

TEST(invoice_column_test, handle_data_copy_constructable)
{
        bool expected{true};
        unsigned int quantity{12};
        std::string description{"Machinging"};
        double amount{5558.99};
        column.set_quantity(quantity);
        column.set_description(description);
        column.set_amount(amount);
        data::column tmp{column};

        CHECK_EQUAL(expected, tmp.is_valid());
}

TEST(invoice_column_test, handle_data_move_assignement)
{
        bool expected{true};
        unsigned int quantity{12};
        std::string description{"Machinging"};
        double amount{5558.99};
        column.set_quantity(quantity);
        column.set_description(description);
        column.set_amount(amount);
        data::column tmp;
        tmp = std::move(column);

        CHECK_EQUAL(expected, tmp.is_valid());
}

TEST(invoice_column_test, handle_data_move_constructable)
{
        bool expected{true};
        unsigned int quantity{12};
        std::string description{"Machinging"};
        double amount{5558.99};
        column.set_quantity(quantity);
        column.set_description(description);
        column.set_amount(amount);
        data::column tmp{std::move(column)};

        CHECK_EQUAL(expected, tmp.is_valid());
}

TEST(invoice_column_test, handle_quantity_data_limit)
{
        bool expected{false};
        unsigned int quantity{UINT_MAX};
        std::string description{"Machinging"};
        double amount{5558.99};
        column.set_quantity(quantity);
        column.set_description(description);
        column.set_amount(amount);

        CHECK_EQUAL(expected, column.is_valid());
}

TEST(invoice_column_test, handle_description_data_limit)
{
        bool expected{false};
        unsigned int quantity{45};
        std::string description{"In software development, the adoption of best"
                " practices plays a crucial role in ensuring maintainability,"
                " readability, and efficiency of codebases. For instance, when"
                " dealing with container classes in C++ such as std::vector,"
                " understanding the behavior of constructors, destructors, "
                "and assignment operators is essential. Consider the case of "
                "passing vectors to functions. When a std::vector is passed by"
                " value, a copy of the vector is made, which could lead to "
                "unnecessary overhead, especially for d fdasfdsfadhgfdshgfdh"};

        double amount{5558.99};
        column.set_quantity(quantity);
        column.set_description(description);
        column.set_amount(amount);

        CHECK_EQUAL(expected, column.is_valid());
}

TEST(invoice_column_test, handle_amount_data_limit)
{
        bool expected{false};
        unsigned int quantity{45};
        std::string description{"Machining"};
        double amount{99999999999999.99};
        column.set_quantity(quantity);
        column.set_description(description);
        column.set_amount(amount);

        CHECK_EQUAL(expected, column.is_valid());
}
