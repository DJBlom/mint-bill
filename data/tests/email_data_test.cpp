/*******************************************************************************
 * Contents: email data unit tests
 * Author: Dawid Blom
 * Date: December 30, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <email_data.h>
#include <client_data.h>
#include <business_data.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Combine business data with client data. (Done)
 * 2) Ensure data copyable. (Done)
 * 3) Ensure data moveable. (Done)
 * 4) Ensure data thread safety. (Done)
 ******************************************************************************/
TEST_GROUP(email_data_test)
{
        data::email email;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(email_data_test, handle_the_business_name)
{
        std::string pdf{"Some pdf dummy data"};
        email.set_pdf(pdf);
        data::client client{};
        client.set_business_name("TME");
        client.set_business_address("Geelsterd 8");
        client.set_business_area_code("543543");
        client.set_business_town_name("George");
        client.set_cellphone_number("0832315944");
        client.set_email("odn@gmail.com");
        client.set_vat_number("3241324321413");
        client.set_statement_schedule("4,4");
        email.set_client(client);

        data::business business{};
        business.set_name("tme");
        business.set_address("geelsterd 8");
        business.set_area_code("5432");
        business.set_town("george");
        business.set_cellphone("0832315944");
        business.set_email("odn@gmail.com");
        business.set_bank("Standard Bank");
        business.set_branch_code("043232");
        business.set_account_number("0932443824");
        business.set_client_message("Thank you for your support");
        business.set_password("fadsfdsafadsf");
        email.set_business(business);

        CHECK_EQUAL(true, email.is_valid());
}

TEST(email_data_test, ensure_copyable)
{
        std::string pdf{"Some pdf dummy data"};
        email.set_pdf(pdf);
        data::client client{};
        client.set_business_name("TME");
        client.set_business_address("Geelsterd 8");
        client.set_business_area_code("543543");
        client.set_business_town_name("George");
        client.set_cellphone_number("0832315944");
        client.set_email("odn@gmail.com");
        client.set_vat_number("3241324321413");
        client.set_statement_schedule("4,4");
        email.set_client(client);

        data::business business{};
        business.set_name("tme");
        business.set_address("geelsterd 8");
        business.set_area_code("5432");
        business.set_town("george");
        business.set_cellphone("0832315944");
        business.set_email("odn@gmail.com");
        business.set_bank("Standard Bank");
        business.set_branch_code("043232");
        business.set_account_number("0932443824");
        business.set_client_message("Thank you for your support");
        business.set_password("fadsfdsafadsf");
        email.set_business(business);
        data::email tmp{};
        tmp = email;


        CHECK_EQUAL(true, tmp.is_valid());
}

TEST(email_data_test, ensure_copy_constructable)
{
        std::string pdf{"Some pdf dummy data"};
        email.set_pdf(pdf);
        data::client client{};
        client.set_business_name("TME");
        client.set_business_address("Geelsterd 8");
        client.set_business_area_code("543543");
        client.set_business_town_name("George");
        client.set_cellphone_number("0832315944");
        client.set_email("odn@gmail.com");
        client.set_vat_number("3241324321413");
        client.set_statement_schedule("4,4");
        email.set_client(client);

        data::business business{};
        business.set_name("tme");
        business.set_address("geelsterd 8");
        business.set_area_code("5432");
        business.set_town("george");
        business.set_cellphone("0832315944");
        business.set_email("odn@gmail.com");
        business.set_bank("Standard Bank");
        business.set_branch_code("043232");
        business.set_account_number("0932443824");
        business.set_client_message("Thank you for your support");
        business.set_password("fadsfdsafadsf");
        email.set_business(business);
        data::email tmp{};
        tmp = email;


        CHECK_EQUAL(true, tmp.is_valid());
}

TEST(email_data_test, ensure_moveable)
{
        std::string pdf{"Some pdf dummy data"};
        email.set_pdf(pdf);

        data::client client{};
        client.set_business_name("TME");
        client.set_business_address("Geelsterd 8");
        client.set_business_area_code("543543");
        client.set_business_town_name("George");
        client.set_cellphone_number("0832315944");
        client.set_email("odn@gmail.com");
        client.set_vat_number("3241324321413");
        client.set_statement_schedule("4,4");
        email.set_client(client);

        data::business business{};
        business.set_name("tme");
        business.set_address("geelsterd 8");
        business.set_area_code("5432");
        business.set_town("george");
        business.set_cellphone("0832315944");
        business.set_email("odn@gmail.com");
        business.set_bank("Standard Bank");
        business.set_branch_code("043232");
        business.set_account_number("0932443824");
        business.set_client_message("Thank you for your support");
        business.set_password("fadsfdsafadsf");
        email.set_business(business);
        data::email tmp{email};


        CHECK_EQUAL(true, tmp.is_valid());
}

TEST(email_data_test, ensure_move_constructable)
{
        std::string pdf{"Some pdf dummy data"};
        email.set_pdf(pdf);
        data::client client{};
        client.set_business_name("TME");
        client.set_business_address("Geelsterd 8");
        client.set_business_area_code("543543");
        client.set_business_town_name("George");
        client.set_cellphone_number("0832315944");
        client.set_email("odn@gmail.com");
        client.set_vat_number("3241324321413");
        client.set_statement_schedule("4,4");
        email.set_client(client);

        data::business business{};
        business.set_name("tme");
        business.set_address("geelsterd 8");
        business.set_area_code("5432");
        business.set_town("george");
        business.set_cellphone("0832315944");
        business.set_email("odn@gmail.com");
        business.set_bank("Standard Bank");
        business.set_branch_code("043232");
        business.set_account_number("0932443824");
        business.set_client_message("Thank you for your support");
        business.set_password("fadsfdsafadsf");
        email.set_business(business);
        data::email tmp{std::move(email)};


        CHECK_EQUAL(true, tmp.is_valid());
}
