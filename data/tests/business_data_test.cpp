/*******************************************************************************
 * Contents: business_data unit tests
 * Author: Dawid Blom
 * Date: November 16, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <business_data.h>
extern "C"
{

}



/**********************************TEST LIST************************************
 * 1) Verify that the data is correct (Done)
 * 2) Verify that the data is incorrect (Done)
 * 3) Get the data from the structure (Done)
 * 4) Ensure that flag bits are cleared properly (Done)
 * 5) Verify the copy ability of the data (Done)
 * 6) Verify the move ability of the data (Done)
 * 7) Verify the email to ensure it's a valid email (Done)
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


TEST(business_data_test, the_data_is_valid)
{
        business_data.set_name("tme");
        business_data.set_address("geelsterd 8");
        business_data.set_area_code("5432");
        business_data.set_town("george");
        business_data.set_cellphone("0832315944");
        business_data.set_email("odn@gmail.com");
        business_data.set_bank("Standard Bank");
        business_data.set_branch_code("043232");
        business_data.set_account_number("0932443824");
        business_data.set_client_message("Thank you for your support");

	CHECK_EQUAL(true, business_data.is_valid());
}

TEST(business_data_test, data_is_not_valid)
{
        business_data.set_name("");
        business_data.set_address("geelsterd 8");
        business_data.set_area_code("");
        business_data.set_town("george");
        business_data.set_cellphone("0832315944");
        business_data.set_email("");
        business_data.set_bank("Standard Bank");
        business_data.set_branch_code("043232");
        business_data.set_account_number("0932443824");
        business_data.set_client_message("Thank you for your support");

	CHECK_EQUAL(false, business_data.is_valid());
}

TEST(business_data_test, fail_when_no_email_is_specified)
{
        business_data.set_name("T.M Engineering");
        business_data.set_address("geelsterd 8");
        business_data.set_area_code("6625");
        business_data.set_town("george");
        business_data.set_cellphone("0832315944");
        business_data.set_email("");
        business_data.set_bank("Standard Bank");
        business_data.set_branch_code("043232");
        business_data.set_account_number("0932443824");
        business_data.set_client_message("Thank you for your support");

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
        business_data.set_bank("Standard Bank");
        business_data.set_branch_code("043232");
        business_data.set_account_number("0932443824");
        business_data.set_client_message("Thank you for your support");

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
        business_data.set_bank("Standard Bank");
        business_data.set_branch_code("043232");
        business_data.set_account_number("0932443824");
        business_data.set_client_message("Thank you for your support");

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
        business_data.set_bank("");
        business_data.set_branch_code("");
        business_data.set_account_number("");
        business_data.set_client_message("");

	CHECK_EQUAL(false, business_data.is_valid());
}

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

TEST(business_data_test, get_business_bank_name_from_structure)
{
        std::string expected{"Bank"};
        std::string bank{expected};
        business_data.set_bank(bank);

        CHECK_EQUAL(expected, business_data.get_bank());
}

TEST(business_data_test, get_business_branch_code_from_structure)
{
        std::string expected{"123"};
        std::string code{expected};
        business_data.set_branch_code(code);

        CHECK_EQUAL(expected, business_data.get_branch_code());
}

TEST(business_data_test, get_business_account_number_from_structure)
{
        std::string expected{"1234566"};
        std::string account{expected};
        business_data.set_account_number(account);

        CHECK_EQUAL(expected, business_data.get_account_number());
}

TEST(business_data_test, get_business_client_message_from_structure)
{
        std::string expected{"Thank, you."};
        std::string message{expected};
        business_data.set_client_message(message);

        CHECK_EQUAL(expected, business_data.get_client_message());
}

TEST(business_data_test, ensure_data_copy_ability)
{
        business_data.set_name("tme");
        business_data.set_address("geelsterd 8");
        business_data.set_area_code("5432");
        business_data.set_town("george");
        business_data.set_cellphone("0832315944");
        business_data.set_email("odn@gmail.com");
        business_data.set_bank("Standard Bank");
        business_data.set_branch_code("043232");
        business_data.set_account_number("0932443824");
        business_data.set_client_message("Thank you for your support");

        data::business tmp_data;
        tmp_data = business_data;


        CHECK_EQUAL(true, tmp_data.is_valid());
}

TEST(business_data_test, ensure_data_move_ability)
{
        business_data.set_name("tme");
        business_data.set_address("geelsterd 8");
        business_data.set_area_code("5432");
        business_data.set_town("george");
        business_data.set_cellphone("0832315944");
        business_data.set_email("odn@gmail.com");
        business_data.set_bank("Standard Bank");
        business_data.set_branch_code("043232");
        business_data.set_account_number("0932443824");
        business_data.set_client_message("Thank you for your support");

        data::business tmp_data;
        tmp_data = std::move(business_data);

        CHECK_EQUAL(true, tmp_data.is_valid());
}

TEST(business_data_test, ensure_email_is_of_valid_format)
{
        business_data.set_name("tme");
        business_data.set_address("geelsterd 8");
        business_data.set_area_code("5432");
        business_data.set_town("george");
        business_data.set_cellphone("0832315944");
        business_data.set_email("odn@gmail.com");
        business_data.set_bank("Standard Bank");
        business_data.set_branch_code("043232");
        business_data.set_account_number("0932443824");
        business_data.set_client_message("Thank you for your support");

        CHECK_EQUAL(true, business_data.is_valid());
}
