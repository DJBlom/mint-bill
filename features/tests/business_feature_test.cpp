/*******************************************************************************
 * Contents: business_feature unit tests
 * Author: Dawid Blom
 * Date: November 18, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include <business_data.h>

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <business_feature.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Load the data from the database.
 * 2) Save the data to the database.
 * 3) Handle the saving of data if the data is not valid
 ******************************************************************************/
TEST_GROUP(business_feature_test)
{
        feature::business business_feature;
	void setup()
	{
	}

	void teardown()
	{
	}
};


TEST(business_feature_test, successfully_load_data_from_database)
{
        data::business data;
        business_feature.load(data);

        CHECK_EQUAL(true, data.is_valid());
}

TEST(business_feature_test, successfully_save_the_data)
{
        data::business data;
        data.set_name("T.M Engineering");
        data.set_address("Geelsterd 8");
        data.set_area_code("6536");
        data.set_town("george");
        data.set_cellphone("0832315944");
        data.set_email("odn@gmail.com");
        data.set_bank("Standard Bank Oudtshoorn");
        data.set_branch_code("050514");
        data.set_account_number("371613191");
        data.set_client_message("Thank you for your support.");

        CHECK_EQUAL(true, business_feature.save(data));
}

TEST(business_feature_test, unsuccessfully_save_the_data)
{
        data::business data;
        data.set_name("T.M Engineering");
        data.set_address("Geelsterd 8");
        data.set_area_code("6536");
        data.set_town("george");
        data.set_cellphone("0832315944");
        data.set_email("");
        data.set_bank("Standard Bank Oudtshoorn");
        data.set_branch_code("050514");
        data.set_account_number("371613191");
        data.set_client_message("Thank you for your support.");

        CHECK_EQUAL(false, business_feature.save(data));
}
