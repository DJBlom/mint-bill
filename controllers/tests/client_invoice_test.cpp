/*******************************************************************************
 * Contents: Invoice feature unit tests
 * Author: Dawid Blom
 * Date: December 9, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <client_invoice.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Load the data from a db.
 * 2) Save the data into a db.
 * 3) Search for data in the db.
 * 4) Send the invoice as an email.
 * 5) Print invoice.
 ******************************************************************************/
TEST_GROUP(client_invoice_test)
{
        feature::client_invoice client_invoice{};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(client_invoice_test, load_data_from_a_db)
{
	data::pdf_invoice data;
	std::vector<std::any> information = client_invoice.load("business name");
	for (const std::any tmp_data : information)
	{
		data = std::any_cast<data::pdf_invoice> (tmp_data);
		break;
	}

        CHECK_EQUAL(true, data.is_valid());
}
