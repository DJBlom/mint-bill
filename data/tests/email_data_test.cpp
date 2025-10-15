/*******************************************************************************
 * Contents: email data unit tests
 * Author: Dawid Blom
 * Date: December 30, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <vector>
#include <email_data.h>
#include <client_data.h>
#include <admin_data.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Combine admin data with client data. (Done)
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
        std::vector<std::string> pdf{};
        pdf.push_back("Some pdf dummy data");
        email.set_attachments(pdf);
        data::client client{};
        client.set_name("TME");
        client.set_address("Geelsterd 8");
        client.set_area_code("543543");
        client.set_town("George");
        client.set_cellphone("0832315944");
        client.set_email("odn@gmail.com");
        client.set_vat_number("3241324321413");
        client.set_statement_schedule("4,4");
        email.set_client(client);

        data::admin admin{};
        admin.set_name("tme");
        admin.set_address("geelsterd 8");
        admin.set_area_code("5432");
        admin.set_town("george");
        admin.set_cellphone("0832315944");
        admin.set_email("odn@gmail.com");
        admin.set_bank("Standard Bank");
        admin.set_branch_code("043232");
        admin.set_account_number("0932443824");
        admin.set_client_message("Thank you for your support");
        admin.set_password("fadsfdsafadsf");
        email.set_business(admin);
        email.set_subject("Invoice");

        CHECK_EQUAL(true, email.is_valid());
}

TEST(email_data_test, handle_limits)
{
        std::vector<std::string> pdf{};
        pdf.push_back("Some pdf dummy data");
        email.set_attachments(pdf);
        data::client client{};
        client.set_name("TME");
        client.set_address("Geelsterd 8");
        client.set_area_code("543543");
        client.set_town("George");
        client.set_cellphone("0832315944");
        client.set_email("odn@gmail.com");
        client.set_vat_number("3241324321413");
        client.set_statement_schedule("4,4");
        email.set_client(client);

        data::admin admin{};
        admin.set_name("tme");
        admin.set_address("geelsterd 8");
        admin.set_area_code("5432");
        admin.set_town("george");
        admin.set_cellphone("0832315944");
        admin.set_email("odn@gmail.com");
        admin.set_bank("Standard Bank");
        admin.set_branch_code("043232");
        admin.set_account_number("0932443824");
        admin.set_client_message("Thank you for your support");
        admin.set_password("fadsfdsafadsf");
        email.set_business(admin);
        std::string file_name{"invoicetkrlsjerkwerkrlijkrldsalkrklwijcklljrewsfjkl"};
        email.set_subject(file_name);

        CHECK_EQUAL(false, email.is_valid());
}

TEST(email_data_test, handle_bad_data)
{
        std::vector<std::string> pdf{};
        pdf.push_back("");
        email.set_attachments(pdf);
        data::client client{};
        client.set_name("");
        client.set_address("");
        client.set_area_code("");
        client.set_town("");
        client.set_cellphone("");
        client.set_email("");
        client.set_vat_number("");
        client.set_statement_schedule("");
        email.set_client(client);

        data::admin admin{};
        admin.set_name("");
        admin.set_address("");
        admin.set_area_code("");
        admin.set_town("");
        admin.set_cellphone("");
        admin.set_email("");
        admin.set_bank("");
        admin.set_branch_code("");
        admin.set_account_number("");
        admin.set_client_message("");
        admin.set_password("");
        email.set_subject("");
        email.set_business(admin);

        CHECK_EQUAL(false, email.is_valid());
}

TEST(email_data_test, handle_no_data)
{
        std::vector<std::string> pdf{};
        pdf.push_back("");
        email.set_attachments(pdf);
        data::client client{};
        email.set_client(client);
        data::admin admin{};
        email.set_business(admin);
        email.set_subject("");

        CHECK_EQUAL(false, email.is_valid());
}

TEST(email_data_test, ensure_copyable)
{
        std::vector<std::string> pdf{};
        pdf.push_back("Some pdf dummy data");
        email.set_attachments(pdf);
        data::client client{};
        client.set_name("TME");
        client.set_address("Geelsterd 8");
        client.set_area_code("543543");
        client.set_town("George");
        client.set_cellphone("0832315944");
        client.set_email("odn@gmail.com");
        client.set_vat_number("3241324321413");
        client.set_statement_schedule("4,4");
        email.set_client(client);

        data::admin admin{};
        admin.set_name("tme");
        admin.set_address("geelsterd 8");
        admin.set_area_code("5432");
        admin.set_town("george");
        admin.set_cellphone("0832315944");
        admin.set_email("odn@gmail.com");
        admin.set_bank("Standard Bank");
        admin.set_branch_code("043232");
        admin.set_account_number("0932443824");
        admin.set_client_message("Thank you for your support");
        admin.set_password("fadsfdsafadsf");
        email.set_business(admin);
        email.set_subject("Invoice");

        data::email tmp{};
        tmp = email;


        CHECK_EQUAL(true, tmp.is_valid());
}

TEST(email_data_test, ensure_copy_constructable)
{
        std::vector<std::string> pdf{};
        pdf.push_back("Some pdf dummy data");
        email.set_attachments(pdf);
        data::client client{};
        client.set_name("TME");
        client.set_address("Geelsterd 8");
        client.set_area_code("543543");
        client.set_town("George");
        client.set_cellphone("0832315944");
        client.set_email("odn@gmail.com");
        client.set_vat_number("3241324321413");
        client.set_statement_schedule("4,4");
        email.set_client(client);

        data::admin admin{};
        admin.set_name("tme");
        admin.set_address("geelsterd 8");
        admin.set_area_code("5432");
        admin.set_town("george");
        admin.set_cellphone("0832315944");
        admin.set_email("odn@gmail.com");
        admin.set_bank("Standard Bank");
        admin.set_branch_code("043232");
        admin.set_account_number("0932443824");
        admin.set_client_message("Thank you for your support");
        admin.set_password("fadsfdsafadsf");
        email.set_subject("Invoice");
        email.set_business(admin);
        data::email tmp{};
        tmp = email;


        CHECK_EQUAL(true, tmp.is_valid());
}

TEST(email_data_test, ensure_moveable)
{
        std::vector<std::string> pdf{};
        pdf.push_back("Some pdf dummy data");
        email.set_attachments(pdf);

        data::client client{};
        client.set_name("TME");
        client.set_address("Geelsterd 8");
        client.set_area_code("543543");
        client.set_town("George");
        client.set_cellphone("0832315944");
        client.set_email("odn@gmail.com");
        client.set_vat_number("3241324321413");
        client.set_statement_schedule("4,4");
        email.set_client(client);

        data::admin admin{};
        admin.set_name("tme");
        admin.set_address("geelsterd 8");
        admin.set_area_code("5432");
        admin.set_town("george");
        admin.set_cellphone("0832315944");
        admin.set_email("odn@gmail.com");
        admin.set_bank("Standard Bank");
        admin.set_branch_code("043232");
        admin.set_account_number("0932443824");
        admin.set_client_message("Thank you for your support");
        admin.set_password("fadsfdsafadsf");
        email.set_subject("Invoice");
        email.set_business(admin);
        data::email tmp{email};


        CHECK_EQUAL(true, tmp.is_valid());
}

TEST(email_data_test, ensure_move_constructable)
{
        std::vector<std::string> pdf{};
        pdf.push_back("Some pdf dummy data");
        email.set_attachments(pdf);
        data::client client{};
        client.set_name("TME");
        client.set_address("Geelsterd 8");
        client.set_area_code("543543");
        client.set_town("George");
        client.set_cellphone("0832315944");
        client.set_email("odn@gmail.com");
        client.set_vat_number("3241324321413");
        client.set_statement_schedule("4,4");
        email.set_client(client);

        data::admin admin{};
        admin.set_name("tme");
        admin.set_address("geelsterd 8");
        admin.set_area_code("5432");
        admin.set_town("george");
        admin.set_cellphone("0832315944");
        admin.set_email("odn@gmail.com");
        admin.set_bank("Standard Bank");
        admin.set_branch_code("043232");
        admin.set_account_number("0932443824");
        admin.set_client_message("Thank you for your support");
        admin.set_password("fadsfdsafadsf");
        email.set_subject("Invoice");
        email.set_business(admin);
        data::email tmp{std::move(email)};


        CHECK_EQUAL(true, tmp.is_valid());
}
