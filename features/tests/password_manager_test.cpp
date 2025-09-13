#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <glib.h>
#include <limits>
#include <future>
#include <errors.h>
#include <iostream>
#include <password_manager.h>
extern "C"
{
}


/**********************************TEST LIST************************************
 * 1) Store a password in the secret-service.
 * 2) Retrieve a password from the secret-service.
 * 3) Ensure that all code is reentrant.
 ******************************************************************************/
TEST_GROUP(password_manager_test)
{
	std::string correct_password{"password"};
	feature::password_manager password_manager{"org.mint-bill.password"};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(password_manager_test, create_password_manager)
{
	CHECK_THROWS(app::errors, feature::password_manager{""});
}

TEST(password_manager_test, fail_to_store_an_empty_password)
{
	bool expected{false};

	CHECK_EQUAL(expected, password_manager.store_password("", 2));
	CHECK_EQUAL(expected, password_manager.store_password(correct_password, std::numeric_limits<int>::max()));
	CHECK_EQUAL(expected, password_manager.store_password(correct_password, std::numeric_limits<int>::min()));
}

TEST(password_manager_test, store_password_in_the_secret_service_success)
{
	bool expected{true};
	bool result{password_manager.store_password(correct_password, 1)};

	CHECK_EQUAL(expected, result);
}

TEST(password_manager_test, lookup_password_in_the_secret_service_fail)
{
	std::string expected{""};

	CHECK_EQUAL(expected, password_manager.lookup_password(2));
	CHECK_EQUAL(expected, password_manager.lookup_password(std::numeric_limits<int>::max()));
	CHECK_EQUAL(expected, password_manager.lookup_password(std::numeric_limits<int>::min()));
}

TEST(password_manager_test, lookup_password_in_the_secret_service_success)
{
	std::string expected{correct_password};

	CHECK_EQUAL(expected, password_manager.lookup_password(1));
}
