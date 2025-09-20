/*******************************************************************************
 * Contents: business feature unit tests
 * Author: Dawid Blom
 * Date: November 18, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <admin_model.h>
#include <admin_data.h>

extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Load the data from the database. (Done)
 * 2) Save the data to the database. (Done)
 ******************************************************************************/
TEST_GROUP(admin_model_test)
{
        data::admin admin_data;
        model::admin admin;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(admin_model_test, unsuccessfully_load_data_from_database)
{
        data::admin data = std::any_cast<data::admin> (admin.load(""));

        CHECK_EQUAL(false, data.is_valid());
}

TEST(admin_model_test, successfully_save_the_data)
{
        admin_data.set_name("tme");
        admin_data.set_address("geelsterd 8");
        admin_data.set_area_code("5432");
        admin_data.set_town("george");
        admin_data.set_cellphone("0832315944");
        admin_data.set_email("odn@gmail.com");
        admin_data.set_bank("Standard Bank");
        admin_data.set_branch_code("043232");
        admin_data.set_account_number("0932443824");
        admin_data.set_password("fdasfdsafdsf");
        admin_data.set_client_message("Thank you for your support");

        CHECK_EQUAL(true, admin.save(admin_data));
}

TEST(admin_model_test, unsuccessfully_save_the_data)
{
        admin_data.set_name("tme");
        admin_data.set_address("geelsterd 8");
        admin_data.set_area_code("5432");
        admin_data.set_town("george");
        admin_data.set_cellphone("0832315944");
        admin_data.set_email("odngmail.com");
        admin_data.set_bank("Standard Bank");
        admin_data.set_branch_code("043232");
        admin_data.set_account_number("0932443824");
        admin_data.set_password("fdasfdsafdsf");
        admin_data.set_client_message("Thank you for your support");

        CHECK_EQUAL(false, admin.save(admin_data));
}
