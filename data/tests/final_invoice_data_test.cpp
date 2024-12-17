/*******************************************************************************
 * Contents: final invoice data unit tests
 * Author: Dawid Blom
 * Date: December 16, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <client_data.h>
#include <column_data.h>
#include <invoice_data.h>
#include <final_invoice_data.h>
extern "C"
{

}

data::client client_information();
std::vector<data::column> create_description_columns();
std::vector<data::column> create_material_columns();
data::invoice invoice_information();
data::business business_information();

/**********************************TEST LIST************************************
 * 1) Ensure data is thread safe. (Done)
 * 2) Ensure data is moveable. (Done)
 * 3) Ensure data is copyable. (Done)
 * 4) Handle the update of client data. (Done)
 * 5) Handle the update of invoice data. (Done)
 * 6) Handle the update of business data. (Done)
 ******************************************************************************/
TEST_GROUP(final_invoice_data_test)
{
        data::final_invoice final_data;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(final_invoice_data_test, handle_combining_the_final_invoice_data)
{
        data::client client_data{client_information()};
        data::invoice invoice_data{invoice_information()};
        data::business business_data{business_information()};
        final_data.update_client_data(client_data);
        final_data.update_invoice_data(invoice_data);
        final_data.update_business_data(business_data);

        CHECK_EQUAL(true, final_data.is_valid());
}

TEST(final_invoice_data_test, handle_final_invoice_data_copy)
{
        data::client client_data{client_information()};
        data::invoice invoice_data{invoice_information()};
        data::business business_data{business_information()};
        final_data.update_client_data(client_data);
        final_data.update_invoice_data(invoice_data);
        final_data.update_business_data(business_data);
        data::final_invoice result = final_data;

        CHECK_EQUAL(true, result.is_valid());
}

TEST(final_invoice_data_test, handle_final_invoice_data_move)
{
        data::client client_data{client_information()};
        data::invoice invoice_data{invoice_information()};
        data::business business_data{business_information()};
        final_data.update_client_data(client_data);
        final_data.update_invoice_data(invoice_data);
        final_data.update_business_data(business_data);
        data::final_invoice result = std::move(final_data);

        CHECK_EQUAL(true, result.is_valid());
}

TEST(final_invoice_data_test, handle_final_invoice_data_copy_construct)
{
        data::client client_data{client_information()};
        data::invoice invoice_data{invoice_information()};
        data::business business_data{business_information()};
        final_data.update_client_data(client_data);
        final_data.update_invoice_data(invoice_data);
        final_data.update_business_data(business_data);
        data::final_invoice result{final_data};

        CHECK_EQUAL(true, result.is_valid());
}

TEST(final_invoice_data_test, handle_final_invoice_data_move_construct)
{
        data::client client_data{client_information()};
        data::invoice invoice_data{invoice_information()};
        data::business business_data{business_information()};
        final_data.update_client_data(client_data);
        final_data.update_invoice_data(invoice_data);
        final_data.update_business_data(business_data);
        data::final_invoice result{std::move(final_data)};

        CHECK_EQUAL(true, result.is_valid());
}

data::client client_information()
{
        data::client data;
        data.set_business_name("client business");
        data.set_business_address("geelsterd 8");
        data.set_business_area_code("6625");
        data.set_business_town_name("George");
        data.set_cellphone_number("0832315944");
        data.set_email("client@gmail.com");
        data.set_vat_number("31439032895L");
        data.set_statement_schedule("5,5");

        return data;
}

std::vector<data::column> create_description_columns()
{
        std::vector<data::column> description_columns{};
        for (unsigned int i = 0; i < 5; ++i)
        {
                data::column column;
                column.set_quantity(i);
                column.set_description("Labor " + std::to_string(i));
                column.set_amount(i * 20);
                description_columns.push_back(column);
        }

        return description_columns;
}

std::vector<data::column> create_material_columns()
{
        std::vector<data::column> material_columns{};
        for (unsigned int i = 0; i < 5; ++i)
        {
                data::column column;
                column.set_quantity(i);
                column.set_description("Material " + std::to_string(i));
                column.set_amount(i * 10);
                material_columns.push_back(column);
        }

        return material_columns;
}

data::invoice invoice_information()
{
        data::invoice data;
        data.set_business_name("client business");
        data.set_invoice_number(1);
        data.set_invoice_date("15-12-2024");
        data.set_job_card_number("24/tk");
        data.set_order_number("24ik");
        data.set_description_total("2345.5");
        data.set_material_total("345.90");
        data.set_grand_total("567.80");
        data.set_description_column(create_description_columns());
        data.set_material_column(create_material_columns());

        return data;
}

data::business business_information()
{
        data::business data;
        data.set_name("test business");
        data.set_address("geelsterd 13");
        data.set_area_code("54246");
        data.set_town("George");
        data.set_cellphone("094569832");
        data.set_email("odn@gmail.com");
        data.set_bank("Nedbank");
        data.set_branch_code("13324");
        data.set_account_number("094832984");
        data.set_client_message("Thank you for your support");

        return data;
}
