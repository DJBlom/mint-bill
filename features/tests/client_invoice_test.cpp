/*******************************************************************************
 * Contents: Invoice feature unit tests
 * Author: Dawid Blom
 * Date: December 9, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <sql.h>
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
        storage::sql db{};
        feature::invoice client_invoice{};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(client_invoice_test, load_data_from_a_db)
{
        data::invoice data{client_invoice.load("business name", db)};

        CHECK_EQUAL(true, data.is_valid());
}

TEST(client_invoice_test, send_invoice_to_client)
{
        const int size{5};
        std::vector<data::column> vec{};
        for (unsigned int i = 0; i < size; ++i)
        {
                data::column expected{};
                expected.set_quantity(i);
                expected.set_description("machining");
                expected.set_amount(55 + i + .0);
                vec.push_back(expected);
        }
        std::string order_number{"order number 123"};
        std::string card_number{"24/md"};
        std::string date{"2023-09-04"};
        std::string name{"Test Business"};
        std::string description_total{"1234.00"};
        std::string material_total{"1234.00"};
        std::string grand_total{"2468.00"};
        std::string number{"1"};

        std::vector<data::invoice> data{};
	for (int i = 0; i < 10; ++i)
	{
		data::invoice invoice;
		invoice.set_business_name(name);
		invoice.set_invoice_number(number);
		invoice.set_invoice_date(date);
		invoice.set_job_card_number(card_number);
		invoice.set_order_number(order_number);
		invoice.set_description_total(description_total);
		invoice.set_material_total(material_total);
		invoice.set_grand_total(grand_total);
		invoice.set_material_column(vec);
		invoice.set_description_column(vec);

		data.push_back(invoice);
	}

        CHECK_EQUAL(true, client_invoice.send_email(data));
}
