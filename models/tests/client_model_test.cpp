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
 * 1) Load data from a db.
 * 2) Save data to a db.
 * 3) Search for a client in the db.
 * 4) Ensure the parsing of the statemen
 ******************************************************************************/
TEST_GROUP(client_model_test)
{
        data::client client_data{};
        model::client client_model{};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(client_model_test, save_data_to_db)
{
        client_data.set_business_name("Dummy");
        client_data.set_business_address("Geelsterd 8");
        client_data.set_business_area_code("05693");
        client_data.set_business_town_name("George");
        client_data.set_cellphone_number("0711422488");
        client_data.set_email("client@gmail.com");
        client_data.set_vat_number("425435");
        client_data.set_statement_schedule("4,4");

        CHECK_EQUAL(true, client_model.save(client_data));
}

TEST(client_model_test, load_data_from_db)
{
        data::client tmp_data{};
        tmp_data = std::any_cast<data::client> (client_model.load("tme"));

        CHECK_EQUAL(true, tmp_data.is_valid());
}
