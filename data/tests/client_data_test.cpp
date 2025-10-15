/*******************************************************************************
 * Contents: client data unit tests
 * Author: Dawid Blom
 * Date: November 26, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <vector>
#include <client_data.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Assign client business name. (Done)
 * 2) Assign street address. (Done)
 * 3) Assign area code. (Done)
 * 4) Assign Town name. (Done)
 * 5) Assign Cellphone number. (Done)
 * 6) Assign Email. (Done)
 * 7) Assign VAT number. (Done)
 * 8) Retrieve client business name. (Done)
 * 9) Retrieve street address. (Done)
 * 10) Retrieve area code. (Done)
 * 11) Retrieve Town name. (Done)
 * 12) Retrieve Cellphone number. (Done)
 * 13) Retrieve Email. (Done)
 * 14) Retrieve VAT number. (Done)
 * 15) Ensure data integrity for multi threaded application. (Done)
 * 16) Ensure no fields are empty. (Done)
 * 17) Ensure email format is correct. (Done)
 * 18) Ensure that proper validation of the statement schedule. (Done)
 * 20) Ensure data is moveable. (Done)
 * 21) Ensure data is copyable. (Done)
 * 22) Assign the statement schedule. (Done)
 * 23) Retrieve the satement schedule. (Done)
 * 24) Entry fields may not hold more than 50 characters. (Done)
 * 25) Ensure that the statement field cannot hold more than 2 numbers. (Done)
 ******************************************************************************/
TEST_GROUP(client_data_test)
{
        data::client client_data{};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(client_data_test, assign_client_business_name_and_retrieve_it)
{
        std::string expected{"tme"};
        client_data.set_name(expected);

        CHECK_EQUAL(expected, client_data.get_name());
}

TEST(client_data_test, assign_client_street_address_and_retrieve_it)
{
        std::string expected{"Geelstred 8"};
        client_data.set_address(expected);

        CHECK_EQUAL(expected, client_data.get_address());
}

TEST(client_data_test, assign_client_area_code_and_retrieve_it)
{
        std::string expected{"6547"};
        client_data.set_area_code(expected);

        CHECK_EQUAL(expected, client_data.get_area_code());
}

TEST(client_data_test, assign_client_town_and_retrieve_it)
{
        std::string expected{"George"};
        client_data.set_town(expected);

        CHECK_EQUAL(expected, client_data.get_town());
}

TEST(client_data_test, assign_client_cellphone_number_and_retrieve_it)
{
        std::string expected{"0832315944"};
        client_data.set_cellphone(expected);

        CHECK_EQUAL(expected, client_data.get_cellphone());
}

TEST(client_data_test, assign_vat_number_and_retrieve_it)
{
        std::string expected{"343214321432"};
        client_data.set_vat_number(expected);

        CHECK_EQUAL(expected, client_data.get_vat_number());
}

TEST(client_data_test, assign_statement_schedule_and_retrieve_it)
{
        std::string expected{"4,4"};
        client_data.set_statement_schedule(expected);

        CHECK_EQUAL(expected, client_data.get_statement_schedule());
}

TEST(client_data_test, ensure_all_fields_are_filled)
{
        client_data.set_name("TME");
        client_data.set_address("Geelsterd 8");
        client_data.set_area_code("543543");
        client_data.set_town("George");
        client_data.set_cellphone("0832315944");
        client_data.set_email("odn@gmail.com");
        client_data.set_vat_number("3241324321413");
        client_data.set_statement_schedule("4,4");

        CHECK_EQUAL(true, client_data.is_valid());
}

TEST(client_data_test, handle_no_fields_filled)
{
        client_data.set_name("");
        client_data.set_address("");
        client_data.set_area_code("");
        client_data.set_town("");
        client_data.set_cellphone("");
        client_data.set_email("");
        client_data.set_vat_number("");
        client_data.set_statement_schedule("");

        CHECK_EQUAL(false, client_data.is_valid());
}

TEST(client_data_test, handle_bad_email_format)
{
        client_data.set_name("TME");
        client_data.set_address("Geelsterd 8");
        client_data.set_area_code("543543");
        client_data.set_town("George");
        client_data.set_cellphone("0832315944");
        client_data.set_email("odngmail.com");
        client_data.set_vat_number("3241324321413");
        client_data.set_statement_schedule("4,4");

        CHECK_EQUAL(false, client_data.is_valid());
}

TEST(client_data_test, handle_bad_schedule_format)
{
        client_data.set_name("TME");
        client_data.set_address("Geelsterd 8");
        client_data.set_area_code("543543");
        client_data.set_town("George");
        client_data.set_cellphone("0832315944");
        client_data.set_email("odn@gmail.com");
        client_data.set_vat_number("3241324321413");
        client_data.set_statement_schedule("4,9");

        CHECK_EQUAL(false, client_data.is_valid());
}

TEST(client_data_test, handle_correct_schedule_format)
{
        client_data.set_name("TME");
        client_data.set_address("Geelsterd 8");
        client_data.set_area_code("543543");
        client_data.set_town("George");
        client_data.set_cellphone("0832315944");
        client_data.set_email("odn@gmail.com");
        client_data.set_vat_number("3241324321413");
        client_data.set_statement_schedule("4,4");

        CHECK_EQUAL(true, client_data.is_valid());
}

TEST(client_data_test, ensure_the_data_is_copyable)
{
        data::client tmp_data{};
        client_data.set_name("TME");
        client_data.set_address("Geelsterd 8");
        client_data.set_area_code("543543");
        client_data.set_town("George");
        client_data.set_cellphone("0832315944");
        client_data.set_email("odn@gmail.com");
        client_data.set_vat_number("3241324321413");
        client_data.set_statement_schedule("4,4");
        tmp_data = client_data;

        CHECK_EQUAL(true, tmp_data.is_valid());
}

TEST(client_data_test, ensure_the_data_is_constructor_copyable)
{
        client_data.set_name("TME");
        client_data.set_address("Geelsterd 8");
        client_data.set_area_code("543543");
        client_data.set_town("George");
        client_data.set_cellphone("0832315944");
        client_data.set_email("odn@gmail.com");
        client_data.set_vat_number("3241324321413");
        client_data.set_statement_schedule("4,4");
        data::client tmp_data{client_data};

        CHECK_EQUAL(true, tmp_data.is_valid());
}

TEST(client_data_test, ensure_the_data_is_movable)
{
        data::client tmp_data{};
        client_data.set_name("TME");
        client_data.set_address("Geelsterd 8");
        client_data.set_area_code("543543");
        client_data.set_town("George");
        client_data.set_cellphone("0832315944");
        client_data.set_email("odn@gmail.com");
        client_data.set_vat_number("3241324321413");
        client_data.set_statement_schedule("4,4");
        tmp_data = std::move(client_data);

        CHECK_EQUAL(true, tmp_data.is_valid());
}

TEST(client_data_test, ensure_the_data_is_constructor_movable)
{
        client_data.set_name("TME");
        client_data.set_address("Geelsterd 8");
        client_data.set_area_code("543543");
        client_data.set_town("George");
        client_data.set_cellphone("0832315944");
        client_data.set_email("odn@gmail.com");
        client_data.set_vat_number("3241324321413");
        client_data.set_statement_schedule("4,4");
        data::client tmp_data{std::move(client_data)};

        CHECK_EQUAL(true, tmp_data.is_valid());
}

TEST(client_data_test, entry_fields_cannot_hold_more_than_fifty_characters)
{
        std::string dummy_data{"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"};
        client_data.set_name(dummy_data);
        client_data.set_address(dummy_data);
        client_data.set_area_code(dummy_data);
        client_data.set_town(dummy_data);
        client_data.set_cellphone(dummy_data);
        client_data.set_email(dummy_data + "odn@gmail.com");
        client_data.set_vat_number(dummy_data);
        client_data.set_statement_schedule("4,4");

        CHECK_EQUAL(false, client_data.is_valid());
}

TEST(client_data_test, entry_fields_hold_less_than_or_equal_to_fifty_characters)
{
        std::string dummy_data{"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"};
        client_data.set_name(dummy_data);
        client_data.set_address(dummy_data);
        client_data.set_area_code(dummy_data);
        client_data.set_town(dummy_data);
        client_data.set_cellphone(dummy_data);
        client_data.set_email("odn@gmail.com odn@gmail.com odn@gmail.com odn@gmail.com odn@gmail.com odn@gmail.com");
        client_data.set_vat_number(dummy_data);
        client_data.set_statement_schedule("4,4");

        CHECK_EQUAL(true, client_data.is_valid());
}

TEST(client_data_test, ensure_that_statement_schedule_cannot_hold_more_than_2_numbers)
{
        std::string dummy_data{"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"};
        client_data.set_name(dummy_data);
        client_data.set_address(dummy_data);
        client_data.set_area_code(dummy_data);
        client_data.set_town(dummy_data);
        client_data.set_cellphone(dummy_data);
        client_data.set_email("odn@gmail.com");
        client_data.set_vat_number(dummy_data);
        client_data.set_statement_schedule("4,4,3");

        CHECK_EQUAL(false, client_data.is_valid());
}
