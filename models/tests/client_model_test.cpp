/*******************************************************************************
 * Contents: Client feature unit tests
 * Author: Dawid Blom
 * Date: November 27, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <client_data.h>
#include <client_model.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Load data from a db. Done
 * 2) Save data to a db. Done
 ******************************************************************************/
TEST_GROUP(client_model_test)
{
	const std::string db_file{"../storage/tests/model_test.db"};
	const std::string db_password{"123456789"};
        data::client client_data{};
        model::client client_model{db_file, db_password};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(client_model_test, unsuccessfully_save_data_to_database)
{
        client_data.set_business_name("Dummy");
        client_data.set_business_address("Geelsterd 12");
        client_data.set_business_area_code("");
        client_data.set_business_town_name("George");
        client_data.set_cellphone_number("0711422488");
        client_data.set_email("client@gmail.com");
        client_data.set_vat_number("425435");
        client_data.set_statement_schedule("4,4");

        CHECK_EQUAL(false, client_model.save(client_data));
}

TEST(client_model_test, successfully_save_data_to_database)
{
        data::client data{};
        data.set_business_name("Dummy");
        data.set_business_address("Geelsterd 8");
        data.set_business_area_code("05693");
        data.set_business_town_name("George");
        data.set_cellphone_number("0711422488");
        data.set_email("dummy@gmail.com");
        data.set_vat_number("4254350");
        data.set_statement_schedule("1,4");

        CHECK_EQUAL(true, client_model.save(data));
}

TEST(client_model_test, successfully_update_data_in_database)
{
        data::client data{};
        data.set_business_name("Dummy");
        data.set_business_address("Geelsterd 12");
        data.set_business_area_code("05693");
        data.set_business_town_name("George");
        data.set_cellphone_number("0711422488");
        data.set_email("clientdummy@gmail.com");
        data.set_vat_number("4254350");
        data.set_statement_schedule("4,4");

        CHECK_EQUAL(true, client_model.save(data));
}

TEST(client_model_test, unsuccessfully_load_data_from_database)
{
        data::client tmp_data{};
        tmp_data = std::any_cast<data::client> (client_model.load(""));

        CHECK_EQUAL(false, tmp_data.is_valid());
}

TEST(client_model_test, successfully_load_data_from_database)
{
        data::client result{};
        data::client data{};
        data.set_business_name("Tests");
        data.set_business_address("Geelsterd 12");
        data.set_business_area_code("05693");
        data.set_business_town_name("George");
        data.set_cellphone_number("0711422488");
        data.set_email("client@gmail.com");
        data.set_vat_number("42543509");
        data.set_statement_schedule("1,4");

	(void) client_model.save(data);
        result = std::any_cast<data::client> (client_model.load("Tests"));

        CHECK_EQUAL(true, result.is_valid());
}
