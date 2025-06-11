/*******************************************************************************
 * Contents: statement data unit tests
 * Author: Dawid Blom
 * Date: February, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <client_data.h>
#include <invoice_data.h>
#include <business_data.h>
#include <statement_data.h>
extern "C"
{

}

/*static data::invoice retrieve_invoice_data();*/
/*static data::business retrieve_business_data();*/
/*static data::client retrieve_client_data();*/

/**********************************TEST LIST************************************
 * 1) The data must be thread safe.
 * 2) The data must contain both the statement column data, the invoice data,
 *    the client data, and the business data.
 * 3) Ensure that the user can copy the information.
 * 4) Ensure that the user can move the data somewhere.
 * 5) The user should be able to assign and retrieve the data.
 ******************************************************************************/
/*TEST_GROUP(statement_data_test)*/
/*{*/
/*	data::client client{};*/
/*	data::business business{};*/
/*        data::statement statement_data{};*/
/*        data::statement_column columns{};*/
/*	void setup()*/
/*	{*/
/*	}*/
/**/
/*	void teardown()*/
/*	{*/
/*	}*/
/*};*/

/*TEST(statement_data_test, assign_and_retrieve_the_statement_column_data)*/
/*{*/
/*        columns.set_invoice_number("1234");*/
/*        columns.set_date("12/04/2025");*/
/*        columns.set_order_number("123 mbd");*/
/*        columns.set_paid_status("Yes");*/
/*        columns.set_price("R1234.00");*/
/*	statement_data.set_columns(columns);*/
/*	data::statement_column result{statement_data.get_columns()};*/
/**/
/*        CHECK_EQUAL(true, result.is_valid());*/
/*}*/
/**/
/*TEST(statement_data_test, assign_and_retrieve_the_business_data)*/
/*{*/
/*	statement_data.set_business(retrieve_business_data());*/
/*	data::business result{statement_data.get_business()};*/
/**/
/*        CHECK_EQUAL(true, result.is_valid());*/
/*}*/
/**/
/*TEST(statement_data_test, assign_and_retrieve_the_client_data)*/
/*{*/
/*	statement_data.set_client(retrieve_client_data());*/
/*	data::client result{statement_data.get_client()};*/
/**/
/*        CHECK_EQUAL(true, result.is_valid());*/
/*}*/
/**/
/*TEST(statement_data_test, assign_and_retrieve_the_pdf_invoice_data)*/
/*{*/
/*        pdf_invoice.set_client(retrieve_client_data());*/
/*        pdf_invoice.set_invoice(retrieve_invoice_data());*/
/*        pdf_invoice.set_business(retrieve_business_data());*/
/**/
/*        CHECK_EQUAL(true, result.is_valid());*/
/*}*/

/*static data::client retrieve_client_data()*/
/*{*/
/*        data::client expected;*/
/*        expected.set_business_name("TME");*/
/*        expected.set_business_address("Geelsterd 8");*/
/*        expected.set_business_area_code("543543");*/
/*        expected.set_business_town_name("George");*/
/*        expected.set_cellphone_number("0832315944");*/
/*        expected.set_email("odn@gmail.com");*/
/*        expected.set_vat_number("3241324321413");*/
/*        expected.set_statement_schedule("4,4");*/
/**/
/*        return expected;*/
/*}*/
/**/
/*static data::invoice retrieve_invoice_data()*/
/*{*/
/*        const int size{50};*/
/*        std::vector<data::column> vec{};*/
/*        for (unsigned int i = 0; i < size; ++i)*/
/*        {*/
/*                data::column expected{};*/
/*                expected.set_quantity(i);*/
/*                expected.set_description("machining");*/
/*                expected.set_amount(55 + i + .0);*/
/*                vec.push_back(expected);*/
/*        }*/
/*        data::invoice expected;*/
/*        expected.set_business_name("Test Business");*/
/*        expected.set_invoice_number("134");*/
/*        expected.set_invoice_date("2023-09-04");*/
/*        expected.set_job_card_number("24/md");*/
/*        expected.set_order_number("order 123");*/
/*        expected.set_description_total("1234.00");*/
/*        expected.set_material_total("1234.00");*/
/*        expected.set_grand_total("1234.00");*/
/*        expected.set_material_column(vec);*/
/*        expected.set_description_column(vec);*/
/**/
/*        return expected;*/
/*}*/
/**/
/*static data::business retrieve_business_data()*/
/*{*/
/*        data::business expected;*/
/*        expected.set_name("tme");*/
/*        expected.set_address("geelsterd 8");*/
/*        expected.set_area_code("5432");*/
/*        expected.set_town("george");*/
/*        expected.set_cellphone("0832315944");*/
/*        expected.set_email("odn@gmail.com");*/
/*        expected.set_bank("Standard Bank");*/
/*        expected.set_branch_code("043232");*/
/*        expected.set_account_number("0932443824");*/
/*        expected.set_client_message("Thank you for your support");*/
/*        expected.set_password("");*/
/**/
/*        return expected;*/
/*}*/
