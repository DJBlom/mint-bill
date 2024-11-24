/*******************************************************************************
 * Contents: business feature unit tests
 * Author: Dawid Blom
 * Date: November 18, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include <business.h>
#include <sql.h>
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Load the data from the database. (Done)
 * 2) Save the data to the database. (Done)
 * 3) Handle the saving of data if the data is not valid. (Done)
 * 4) Verify that the data is correct. (Done)
 * 5) Verify that the data is incorrect. (Done)
 * 6) Get the data from the structure. (Done)
 * 7) Ensure that flag bits are cleared properly. (Done)
 * 8) Verify the copy ability of the data. (Done)
 * 9) Verify the move ability of the data. (Done)
 * 10) Verify the email to ensure it's a valid email. (Done)
 ******************************************************************************/
TEST_GROUP(business_feature_test)
{
        storage::sql sql;
        feature::business business_feature;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(business_feature_test, the_feature_is_valid)
{
        business_feature.set_name("tme");
        business_feature.set_address("geelsterd 8");
        business_feature.set_area_code("5432");
        business_feature.set_town("george");
        business_feature.set_cellphone("0832315944");
        business_feature.set_email("odn@gmail.com");
        business_feature.set_bank("Standard Bank");
        business_feature.set_branch_code("043232");
        business_feature.set_account_number("0932443824");
        business_feature.set_client_message("Thank you for your support");

	CHECK_EQUAL(true, business_feature.is_valid());
}

TEST(business_feature_test, data_is_not_valid)
{
        business_feature.set_name("");
        business_feature.set_address("geelsterd 8");
        business_feature.set_area_code("");
        business_feature.set_town("george");
        business_feature.set_cellphone("0832315944");
        business_feature.set_email("");
        business_feature.set_bank("Standard Bank");
        business_feature.set_branch_code("043232");
        business_feature.set_account_number("0932443824");
        business_feature.set_client_message("Thank you for your support");

	CHECK_EQUAL(false, business_feature.is_valid());
}

TEST(business_feature_test, fail_when_no_email_is_specified)
{
        business_feature.set_name("T.M Engineering");
        business_feature.set_address("geelsterd 8");
        business_feature.set_area_code("6625");
        business_feature.set_town("george");
        business_feature.set_cellphone("0832315944");
        business_feature.set_email("");
        business_feature.set_bank("Standard Bank");
        business_feature.set_branch_code("043232");
        business_feature.set_account_number("0932443824");
        business_feature.set_client_message("Thank you for your support");

	CHECK_EQUAL(false, business_feature.is_valid());
}

TEST(business_feature_test, fail_when_wrong_email_format_is_used)
{
        business_feature.set_name("T.M Engineering");
        business_feature.set_address("geelsterd 8");
        business_feature.set_area_code("6625");
        business_feature.set_town("george");
        business_feature.set_cellphone("0832315944");
        business_feature.set_email("odngmail.com");
        business_feature.set_bank("Standard Bank");
        business_feature.set_branch_code("043232");
        business_feature.set_account_number("0932443824");
        business_feature.set_client_message("Thank you for your support");

	CHECK_EQUAL(false, business_feature.is_valid());
}

TEST(business_feature_test, pass_when_correct_email_format_is_used)
{
        business_feature.set_name("T.M Engineering");
        business_feature.set_address("geelsterd 8");
        business_feature.set_area_code("6625");
        business_feature.set_town("george");
        business_feature.set_cellphone("0832315944");
        business_feature.set_email("odn@gmail.com");
        business_feature.set_bank("Standard Bank");
        business_feature.set_branch_code("043232");
        business_feature.set_account_number("0932443824");
        business_feature.set_client_message("Thank you for your support");

	CHECK_EQUAL(true, business_feature.is_valid());
}

TEST(business_feature_test, verify_flag_bits_are_properly_cleared)
{
        business_feature.set_name("");
        business_feature.set_address("");
        business_feature.set_area_code("");
        business_feature.set_town("");
        business_feature.set_cellphone("");
        business_feature.set_email("");
        business_feature.set_bank("");
        business_feature.set_branch_code("");
        business_feature.set_account_number("");
        business_feature.set_client_message("");

	CHECK_EQUAL(false, business_feature.is_valid());
}

TEST(business_feature_test, successfully_load_data_from_database)
{
        CHECK_EQUAL(true, business_feature.load(sql));
}

TEST(business_feature_test, successfully_save_the_data)
{
        business_feature.set_name("tme");
        business_feature.set_address("geelsterd 8");
        business_feature.set_area_code("5432");
        business_feature.set_town("george");
        business_feature.set_cellphone("0832315944");
        business_feature.set_email("odn@gmail.com");
        business_feature.set_bank("Standard Bank");
        business_feature.set_branch_code("043232");
        business_feature.set_account_number("0932443824");
        business_feature.set_client_message("Thank you for your support");

        CHECK_EQUAL(true, business_feature.save(sql));
}

TEST(business_feature_test, unsuccessfully_save_the_data)
{
        business_feature.set_name("tme");
        business_feature.set_address("geelsterd 8");
        business_feature.set_area_code("5432");
        business_feature.set_town("george");
        business_feature.set_cellphone("0832315944");
        business_feature.set_email("");
        business_feature.set_bank("Standard Bank");
        business_feature.set_branch_code("043232");
        business_feature.set_account_number("0932443824");
        business_feature.set_client_message("Thank you for your support");

        CHECK_EQUAL(false, business_feature.save(sql));
}

TEST(business_feature_test, get_business_name_from_structure)
{
        std::string expected{"tme"};
        std::string name{expected};
        business_feature.set_name(name);

        CHECK_EQUAL(expected, business_feature.get_name());
}

TEST(business_feature_test, get_business_address_from_structure)
{
        std::string expected{"geelsterd 8"};
        std::string address{expected};
        business_feature.set_address(address);

        CHECK_EQUAL(expected, business_feature.get_address());
}

TEST(business_feature_test, get_business_area_code_from_structure)
{
        std::string expected{"5467"};
        std::string code{expected};
        business_feature.set_area_code(code);

        CHECK_EQUAL(expected, business_feature.get_area_code());
}

TEST(business_feature_test, get_business_town_from_structure)
{
        std::string expected{"george"};
        std::string town{expected};
        business_feature.set_town(town);

        CHECK_EQUAL(expected, business_feature.get_town());
}

TEST(business_feature_test, get_business_cellphone_number_from_structure)
{
        std::string expected{"0823215944"};
        std::string cellphone{expected};
        business_feature.set_cellphone(cellphone);

        CHECK_EQUAL(expected, business_feature.get_cellphone());
}

TEST(business_feature_test, get_business_email_address_from_structure)
{
        std::string expected{"odn@gmail.com"};
        std::string email{expected};
        business_feature.set_email(email);

        CHECK_EQUAL(expected, business_feature.get_email());
}

TEST(business_feature_test, get_business_bank_name_from_structure)
{
        std::string expected{"Bank"};
        std::string bank{expected};
        business_feature.set_bank(bank);

        CHECK_EQUAL(expected, business_feature.get_bank());
}

TEST(business_feature_test, get_business_branch_code_from_structure)
{
        std::string expected{"123"};
        std::string code{expected};
        business_feature.set_branch_code(code);

        CHECK_EQUAL(expected, business_feature.get_branch_code());
}

TEST(business_feature_test, get_business_account_number_from_structure)
{
        std::string expected{"1234566"};
        std::string account{expected};
        business_feature.set_account_number(account);

        CHECK_EQUAL(expected, business_feature.get_account_number());
}

TEST(business_feature_test, get_business_client_message_from_structure)
{
        std::string expected{"Thank, you."};
        std::string message{expected};
        business_feature.set_client_message(message);

        CHECK_EQUAL(expected, business_feature.get_client_message());
}

TEST(business_feature_test, ensure_feature_copy_ability)
{
        business_feature.set_name("tme");
        business_feature.set_address("geelsterd 8");
        business_feature.set_area_code("5432");
        business_feature.set_town("george");
        business_feature.set_cellphone("0832315944");
        business_feature.set_email("odn@gmail.com");
        business_feature.set_bank("Standard Bank");
        business_feature.set_branch_code("043232");
        business_feature.set_account_number("0932443824");
        business_feature.set_client_message("Thank you for your support");

        feature::business tmp_feature;
        tmp_feature = business_feature;


        CHECK_EQUAL(true, tmp_feature.is_valid());
}

TEST(business_feature_test, ensure_data_move_ability)
{
        business_feature.set_name("tme");
        business_feature.set_address("geelsterd 8");
        business_feature.set_area_code("5432");
        business_feature.set_town("george");
        business_feature.set_cellphone("0832315944");
        business_feature.set_email("odn@gmail.com");
        business_feature.set_bank("Standard Bank");
        business_feature.set_branch_code("043232");
        business_feature.set_account_number("0932443824");
        business_feature.set_client_message("Thank you for your support");

        feature::business tmp_feature;
        tmp_feature = std::move(business_feature);

        CHECK_EQUAL(true, tmp_feature.is_valid());
}
