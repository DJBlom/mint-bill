/*******************************************************************************
 * Contents: business feature unit tests
 * Author: Dawid Blom
 * Date: November 18, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include <business_info.h>
#include <business_data.h>
#include <sql.h>
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Load the data from the database. (Done)
 * 2) Save the data to the database. (Done)
 ******************************************************************************/
TEST_GROUP(business_feature_test)
{
        storage::sql sql;
        data::business business_data;
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
        data::business data = business_feature.load(sql);

        CHECK_EQUAL(true, data.is_valid());
}

TEST(business_feature_test, successfully_save_the_data)
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

        CHECK_EQUAL(true, business_feature.save(business_data, sql));
}

TEST(business_feature_test, unsuccessfully_save_the_data)
{
        business_data.set_name("tme");
        business_data.set_address("geelsterd 8");
        business_data.set_area_code("5432");
        business_data.set_town("george");
        business_data.set_cellphone("0832315944");
        business_data.set_email("");
        business_data.set_bank("Standard Bank");
        business_data.set_branch_code("043232");
        business_data.set_account_number("0932443824");
        business_data.set_client_message("Thank you for your support");

        CHECK_EQUAL(false, business_feature.save(business_data, sql));
}
