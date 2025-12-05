/*****************************************************************************
 * @file business_data_test.cpp
 *
 * @brief
 *   Unit tests for the data::business model, verifying correctness,
 *   validation logic, and copy/move behavior of the core business data layer.
 *
 * @details
 *   This test suite exercises the foundational business data structure, which
 *   represents shared contact and identity information used across multiple
 *   higher-level models. The following aspects are covered:
 *
 *   - Field assignment and retrieval:
 *       • name
 *       • address
 *       • area code
 *       • town
 *       • cellphone number
 *       • email address
 *
 *   - Validation behavior:
 *       • Required fields must not be empty
 *       • Email addresses must match the expected format
 *       • Entry fields must respect the maximum length constraints
 *
 *   - Flag and state handling:
 *       • Ensures that flag bits are properly set and cleared
 *       • Confirms that is_valid() reflects the true completeness of data
 *
 *   - Copy and move semantics:
 *       • Validates that copied and moved instances preserve correctness
 *         and the internal validity state.
 *
 *   Together, these tests help ensure that data::business behaves reliably as
 *   the base class for extended models such as client and admin.
 *****************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <business_data.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Verify that the data is correct. (Done)
 * 2) Verify that the data is incorrect. (Done)
 * 3) Get the data from the structure. (Done)
 * 4) Ensure that flag bits are cleared properly. (Done)
 * 5) Verify the copy ability of the data. (Done)
 * 6) Verify the move ability of the data. (Done)
 * 7) Verify the email to ensure it's a valid email. (Done)
 * 8) Entry fields may not hold more than 50 characters. (Done)
 ******************************************************************************/
TEST_GROUP(business_data_test)
{
        data::business business_data;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(business_data_test, get_business_name_from_structure)
{
        std::string expected{"tme"};
        std::string name{expected};
        business_data.set_name(name);

        CHECK_EQUAL(expected, business_data.get_name());
}

TEST(business_data_test, get_business_address_from_structure)
{
        std::string expected{"geelsterd 8"};
        std::string address{expected};
        business_data.set_address(address);

        CHECK_EQUAL(expected, business_data.get_address());
}

TEST(business_data_test, get_business_area_code_from_structure)
{
        std::string expected{"5467"};
        std::string code{expected};
        business_data.set_area_code(code);

        CHECK_EQUAL(expected, business_data.get_area_code());
}

TEST(business_data_test, get_business_town_from_structure)
{
        std::string expected{"george"};
        std::string town{expected};
        business_data.set_town(town);

        CHECK_EQUAL(expected, business_data.get_town());
}

TEST(business_data_test, get_business_cellphone_number_from_structure)
{
        std::string expected{"0823215944"};
        std::string cellphone{expected};
        business_data.set_cellphone(cellphone);

        CHECK_EQUAL(expected, business_data.get_cellphone());
}

TEST(business_data_test, get_business_email_address_from_structure)
{
        std::string expected{"odn@gmail.com"};
        std::string email{expected};
        business_data.set_email(email);

        CHECK_EQUAL(expected, business_data.get_email());
}

TEST(business_data_test, data_is_not_valid)
{
        business_data.set_name("");
        business_data.set_address("geelsterd 8");
        business_data.set_area_code("5432");
        business_data.set_town("george");
        business_data.set_cellphone("0832315944");
        business_data.set_email("odn@gmail.com");

	CHECK_EQUAL(false, business_data.is_valid());
}

TEST(business_data_test, valid_data)
{
        business_data.set_name("tme");
        business_data.set_address("geelsterd 8");
        business_data.set_area_code("5432");
        business_data.set_town("george");
        business_data.set_cellphone("0832315944");
        business_data.set_email("odn@gmail.com");


	CHECK_EQUAL(true, business_data.is_valid());
}

TEST(business_data_test, fail_when_no_email_is_specified)
{
        business_data.set_name("T.M Engineering");
        business_data.set_address("geelsterd 8");
        business_data.set_area_code("6625");
        business_data.set_town("george");
        business_data.set_cellphone("0832315944");
        business_data.set_email("");

	CHECK_EQUAL(false, business_data.is_valid());
}

TEST(business_data_test, fail_when_wrong_email_format_is_used)
{
        business_data.set_name("T.M Engineering");
        business_data.set_address("geelsterd 8");
        business_data.set_area_code("6625");
        business_data.set_town("george");
        business_data.set_cellphone("0832315944");
        business_data.set_email("odngmail.com");

	CHECK_EQUAL(false, business_data.is_valid());
}

TEST(business_data_test, pass_when_correct_email_format_is_used)
{
        business_data.set_name("T.M Engineering");
        business_data.set_address("geelsterd 8");
        business_data.set_area_code("6625");
        business_data.set_town("george");
        business_data.set_cellphone("0832315944");
        business_data.set_email("odn@gmail.com");

	CHECK_EQUAL(true, business_data.is_valid());
}

TEST(business_data_test, verify_flag_bits_are_properly_cleared)
{
        business_data.set_name("");
        business_data.set_address("");
        business_data.set_area_code("");
        business_data.set_town("");
        business_data.set_cellphone("");
        business_data.set_email("");

	CHECK_EQUAL(false, business_data.is_valid());
}

TEST(business_data_test, ensure_feature_copy_ability)
{
        business_data.set_name("tme");
        business_data.set_address("geelsterd 8");
        business_data.set_area_code("5432");
        business_data.set_town("george");
        business_data.set_cellphone("0832315944");
        business_data.set_email("odn@gmail.com");
        data::business tmp_feature;
        tmp_feature = business_data;


        CHECK_EQUAL(true, tmp_feature.is_valid());
}

TEST(business_data_test, ensure_data_move_ability)
{
        business_data.set_name("T.M. Engineering");
        business_data.set_address("Geelsterd 8");
        business_data.set_area_code("5432");
        business_data.set_town("George");
        business_data.set_cellphone("0832315944");
        business_data.set_email("odn@gmail.com");
        data::business tmp_feature;
        tmp_feature = std::move(business_data);

        CHECK_EQUAL(true, tmp_feature.is_valid());
}

TEST(business_data_test, entry_fields_hold_less_than_or_equal_to_fifty_characters)
{
        std::string dummy_data{"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"};
        business_data.set_name(dummy_data);
        business_data.set_address(dummy_data);
        business_data.set_area_code(dummy_data);
        business_data.set_town(dummy_data);
        business_data.set_cellphone(dummy_data);
        business_data.set_email("odn@gmail.com");

        CHECK_EQUAL(true, business_data.is_valid());
}

TEST(business_data_test, entry_fields_cannot_hold_more_than_fifty_characters)
{
        std::string dummy_data{"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"};
        business_data.set_name(dummy_data);
        business_data.set_address(dummy_data);
        business_data.set_area_code(dummy_data);
        business_data.set_town(dummy_data);
        business_data.set_cellphone(dummy_data);
        business_data.set_email(dummy_data + "@gmail.com");

        CHECK_EQUAL(false, business_data.is_valid());
}

TEST(business_data_test, client_message_is_less_than_or_equal_to_100_characters)
{
        std::string dummy_data{"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"};
        business_data.set_name(dummy_data);
        business_data.set_address(dummy_data);
        business_data.set_area_code(dummy_data);
        business_data.set_town(dummy_data);
        business_data.set_cellphone(dummy_data);
        business_data.set_email("odn@gmail.com");

        CHECK_EQUAL(true, business_data.is_valid());
}

TEST(business_data_test, verify_data_limits)
{
        std::string dummy_data{"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"};
        business_data.set_name(dummy_data);
        business_data.set_address(dummy_data);
        business_data.set_area_code(dummy_data);
        business_data.set_town(dummy_data);
        business_data.set_cellphone(dummy_data);
        business_data.set_email("odn@gmail" + dummy_data + ".com");

        CHECK_EQUAL(false, business_data.is_valid());
}
