/*******************************************************************************
 * Contents: billing data unit tests
 * Author: Dawid Blom
 * Date: November 27, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <iostream>
#include <vector>
#include <billing_data.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Assign the business name searched for. (Done)
 * 2) Retrieve the business name searched for. (Done)
 * 3) Assign the start period. (Done)
 * 4) Retrieve the start period. (Done)
 * 5) Assign the end period. (Done)
 * 6) Retrieve the end period. (Done)
 * 7) Assign the schedule. (Done)
 * 8) Retrieve the schedule. (Done)
 * 9) Assign the billing date. (Done)
 * 10) Retrieve the billing date. (Done)
 * 11) Ensure none of the fields are empty. (Done)
 * 12) All fields should be contraint to 50 characters except the description
 *     column field in both the column views. (Done)
 * 13) Ensure the data is copyable. (Done)
 * 14) Ensure the data is moveable. (Done)
 * 15) Ensure the data is thread safe. (Done)
 * 16) Assign the created date. (Done)
 * 17) Retrieve the created date. (Done)
 * 18) Assign the paid_status date. (Done)
 * 19) Retrieve the paid_status date. (Done)
 ******************************************************************************/
TEST_GROUP(billing_data_test)
{
        data::billing billing_data;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(billing_data_test, handle_the_billing_id)
{
        std::string expected{"1"};
	billing_data.set_id(expected);
        std::string result{billing_data.get_id()};

        CHECK_EQUAL(expected, result);
}

TEST(billing_data_test, handle_the_business_name)
{
        std::string expected{"Test Business"};
        billing_data.set_name(expected);
        std::string result{billing_data.get_name()};

        STRCMP_EQUAL(expected.c_str(), result.c_str());
}

TEST(billing_data_test, handle_the_statement_date)
{
        std::string expected{"Nov-5-2025"};
	billing_data.set_date(expected);
        std::string result{billing_data.get_date()};

        CHECK_EQUAL(expected, result);
}

TEST(billing_data_test, handle_the_paid_status)
{
        std::string expected{"Not Paid"};
	billing_data.set_paid_status(expected);
        std::string result{billing_data.get_paid_status()};

        CHECK_EQUAL(expected, result);
}

TEST(billing_data_test, handle_the_copy_of_the_data)
{
	billing_data.set_id("1");
	billing_data.set_name("test business");
	billing_data.set_date("Nov-5-2025");
	billing_data.set_paid_status("Not Paid");
        data::billing result;
        result = billing_data;

        CHECK_EQUAL(true, result.is_valid());
}

TEST(billing_data_test, handle_the_move_of_the_data)
{
	billing_data.set_id("1");
	billing_data.set_name("test business");
	billing_data.set_date("Nov-5-2025");
	billing_data.set_paid_status("Not Paid");
        data::billing result;
        result = std::move(billing_data);

        CHECK_EQUAL(true, result.is_valid());
}

TEST(billing_data_test, handle_copy_constructing_of_the_data)
{
	billing_data.set_id("1");
	billing_data.set_name("test business");
	billing_data.set_date("Nov-5-2025");
	billing_data.set_paid_status("Not Paid");
        data::billing result{billing_data};

        CHECK_EQUAL(true, result.is_valid());
}

TEST(billing_data_test, handle_move_constructing_of_the_data)
{
	billing_data.set_id("1");
	billing_data.set_name("test business");
	billing_data.set_date("Nov-5-2025");
	billing_data.set_paid_status("Not Paid");
        data::billing result{std::move(billing_data)};

        CHECK_EQUAL(true, result.is_valid());
}

TEST(billing_data_test, handle_failure_to_set_data)
{
	billing_data.set_id("");
	billing_data.set_name("");
	billing_data.set_date("");
	billing_data.set_paid_status("");

        CHECK_EQUAL(false, billing_data.is_valid());
}

TEST(billing_data_test, handle_data_limits)
{
	std::string limit{"This position corresponds closely with"
			  "the skills and technologies I aim to"
			  "strengthen as I shape the next phase of my career."};
	billing_data.set_id(limit);
	billing_data.set_name(limit);
	billing_data.set_date(limit);
	billing_data.set_paid_status(limit);

        CHECK_EQUAL(false, billing_data.is_valid());
}

TEST(billing_data_test, handle_bad_data)
{
	billing_data.set_id("1");
	billing_data.set_name("test business");
	billing_data.set_date("Nov-5-2025");
	billing_data.set_paid_status("");

        CHECK_EQUAL(false, billing_data.is_valid());
}
