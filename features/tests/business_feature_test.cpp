/*******************************************************************************
 * Contents: admin feature unit tests
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
TEST_GROUP(admin_controller_test)
{
        data::admin business_data;
        model::admin admin_model;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(admin_controller_test, successfully_load_data_from_database)
{
        data::admin data = std::any_cast<data::admin>(admin_model.load("tme"));

        CHECK_EQUAL(true, data.is_valid());
}

TEST(admin_controller_test, successfully_save_the_data)
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
        business_data.set_password("fdasfdsafdsf");
        business_data.set_client_message("Thank you for your support");

        CHECK_EQUAL(true, admin_model.save(business_data));
}

TEST(admin_controller_test, unsuccessfully_save_the_data)
{
        business_data.set_name("tme");
        business_data.set_address("geelsterd 8");
        business_data.set_area_code("5432");
        business_data.set_town("george");
        business_data.set_cellphone("0832315944");
        business_data.set_email("odngmail.com");
        business_data.set_bank("Standard Bank");
        business_data.set_branch_code("043232");
        business_data.set_account_number("0932443824");
        business_data.set_password("fdasfdsafdsf");
        business_data.set_client_message("Thank you for your support");

        CHECK_EQUAL(false, admin_model.save(business_data));
}
