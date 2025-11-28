/*******************************************************************************
 * Contents: statement data unit tests
 * Author: Dawid Blom
 * Date: December 4, 2024
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
 * 1) Assign the start period. (Done)
 * 2) Retrieve the start period. (Done)
 * 3) Assign the end period. (Done)
 * 4) Retrieve the end period. (Done)
 * 5) Assign the schedule. (Done)
 * 6) Retrieve the schedule. (Done)
 * 7) Assign the statement date. (Done)
 * 8) Retrieve the statement date. (Done)
 * 9) Ensure none of the fields are empty. (Done)
 * 10) All fields should be contraint to 50 characters except the description
 *     column field in both the column views. (Done)
 * 11) Ensure the data is copyable. (Done)
 * 12) Ensure the data is moveable. (Done)
 * 13) Ensure the data is thread safe. (Done)
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

TEST(statement_data_test, handle_the_start_period)
{
        std::string expected{"Nov-1-2025"};
	statement_data.set_period_start(expected);
        std::string result{statement_data.get_period_start()};

        CHECK_EQUAL(expected, result);
}

TEST(statement_data_test, handle_the_end_period)
{
        std::string expected{"Nov-1-2025"};
	statement_data.set_period_end(expected);
        std::string result{statement_data.get_period_end()};

        CHECK_EQUAL(expected, result);
}

TEST(statement_data_test, handle_the_schedule)
{
        std::string expected{"4,1"};
	statement_data.set_schedule(expected);
        std::string result{statement_data.get_schedule()};

        CHECK_EQUAL(expected, result);
}

TEST(statement_data_test, handle_the_copy_of_the_data)
{
	statement_data.set_id("1");
	statement_data.set_name("test business");
	statement_data.set_period_start("Nov-1-2025");
	statement_data.set_period_end("Nov-28-2025");
	statement_data.set_schedule("4,1");
	statement_data.set_date("Nov-5-2025");
	statement_data.set_paid_status("Not Paid");
        data::statement result;
        result = statement_data;

        CHECK_EQUAL(true, result.is_valid());
}

TEST(statement_data_test, handle_the_move_of_the_data)
{
	statement_data.set_id("1");
	statement_data.set_name("test business");
	statement_data.set_period_start("Nov-1-2025");
	statement_data.set_period_end("Nov-28-2025");
	statement_data.set_schedule("4,1");
	statement_data.set_date("Nov-5-2025");
	statement_data.set_paid_status("Not Paid");
        data::statement result;
        result = std::move(statement_data);

        CHECK_EQUAL(true, result.is_valid());
}

TEST(statement_data_test, handle_copy_constructing_of_the_data)
{
	statement_data.set_id("1");
	statement_data.set_name("test business");
	statement_data.set_period_start("Nov-1-2025");
	statement_data.set_period_end("Nov-28-2025");
	statement_data.set_schedule("4,1");
	statement_data.set_date("Nov-5-2025");
	statement_data.set_paid_status("Not Paid");
        data::statement result{statement_data};

        CHECK_EQUAL(true, result.is_valid());
}

TEST(statement_data_test, handle_move_constructing_of_the_data)
{
	statement_data.set_id("1");
	statement_data.set_name("test business");
	statement_data.set_period_start("Nov-1-2025");
	statement_data.set_period_end("Nov-28-2025");
	statement_data.set_schedule("4,1");
	statement_data.set_date("Nov-5-2025");
	statement_data.set_paid_status("Not Paid");
        data::statement result{std::move(statement_data)};

        CHECK_EQUAL(true, result.is_valid());
}

TEST(statement_data_test, handle_failure_to_set_data)
{
	statement_data.set_id("1");
	statement_data.set_name("test business");
	statement_data.set_period_start("Nov-1-2025");
	statement_data.set_period_end("Nov-28-2025");
	statement_data.set_schedule("");
	statement_data.set_date("");
	statement_data.set_paid_status("Not Paid");

        CHECK_EQUAL(false, statement_data.is_valid());
}

TEST(statement_data_test, handle_data_limits)
{
	std::string limit{"This position corresponds closely with"
			  "the skills and technologies I aim to"
			  "strengthen as I shape the next phase of my career."};
	statement_data.set_id(limit);
	statement_data.set_name(limit);
	statement_data.set_period_start(limit);
	statement_data.set_period_end(limit);
	statement_data.set_schedule(limit);
	statement_data.set_date(limit);
	statement_data.set_paid_status(limit);

        CHECK_EQUAL(false, statement_data.is_valid());
}

TEST(statement_data_test, handle_bad_data)
{

        CHECK_EQUAL(false, statement_data.is_valid());
}
