/*******************************************************************************
 * Contents: Date manger unit tests
 * Author: Dawid Blom
 * Date: November 28, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <date_manager.h>
#include <iostream>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1)
 ******************************************************************************/
TEST_GROUP(date_manager_test)
{
	utility::date_manager date_manager;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(date_manager_test, get_current_date)
{
	std::string date{date_manager.current_date()};
	std::cout << "DATE_MANAGER: " << date << std::endl;

	CHECK_EQUAL(false, date.empty());
}

TEST(date_manager_test, empty_schedule)
{
	utility::period_bounds expected;
	utility::period_bounds result{date_manager.compute_period_bounds("")};

	CHECK_EQUAL(expected.period_start, result.period_start);
	CHECK_EQUAL(expected.period_end, result.period_end);
}

TEST(date_manager_test, statement_period_1_1)
{
	utility::period_bounds period{date_manager.compute_period_bounds("1,1")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_1_2)
{
	utility::period_bounds period{date_manager.compute_period_bounds("1,2")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_1_3)
{
	utility::period_bounds period{date_manager.compute_period_bounds("1,3")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_1_4)
{
	utility::period_bounds period{date_manager.compute_period_bounds("1,4")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_1_5)
{
	utility::period_bounds period{date_manager.compute_period_bounds("1,5")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_1_6)
{
	utility::period_bounds period{date_manager.compute_period_bounds("1,6")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_1_7)
{
	utility::period_bounds period{date_manager.compute_period_bounds("1,7")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_2_1)
{
	utility::period_bounds period{date_manager.compute_period_bounds("2,1")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_2_2)
{
	utility::period_bounds period{date_manager.compute_period_bounds("2,2")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_2_3)
{
	utility::period_bounds period{date_manager.compute_period_bounds("2,3")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_2_4)
{
	utility::period_bounds period{date_manager.compute_period_bounds("2,4")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_2_5)
{
	utility::period_bounds period{date_manager.compute_period_bounds("2,5")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_2_6)
{
	utility::period_bounds period{date_manager.compute_period_bounds("2,6")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_2_7)
{
	utility::period_bounds period{date_manager.compute_period_bounds("2,7")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_3_1)
{
	utility::period_bounds period{date_manager.compute_period_bounds("3,1")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_3_2)
{
	utility::period_bounds period{date_manager.compute_period_bounds("3,2")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_3_3)
{
	utility::period_bounds period{date_manager.compute_period_bounds("3,3")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_3_4)
{
	utility::period_bounds period{date_manager.compute_period_bounds("3,4")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_3_5)
{
	utility::period_bounds period{date_manager.compute_period_bounds("3,5")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_3_6)
{
	utility::period_bounds period{date_manager.compute_period_bounds("3,6")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_3_7)
{
	utility::period_bounds period{date_manager.compute_period_bounds("3,7")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_4_1)
{
	utility::period_bounds period{date_manager.compute_period_bounds("4,1")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_4_2)
{
	utility::period_bounds period{date_manager.compute_period_bounds("4,2")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_4_3)
{
	utility::period_bounds period{date_manager.compute_period_bounds("4,3")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_4_4)
{
	utility::period_bounds period{date_manager.compute_period_bounds("4,4")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_4_5)
{
	utility::period_bounds period{date_manager.compute_period_bounds("4,5")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_4_6)
{
	utility::period_bounds period{date_manager.compute_period_bounds("4,6")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}

TEST(date_manager_test, statement_period_4_7)
{
	utility::period_bounds period{date_manager.compute_period_bounds("4,7")};
	std::cout << "Start Period: " << period.period_start << std::endl;
	std::cout << "End Period: " << period.period_end << std::endl;
}
