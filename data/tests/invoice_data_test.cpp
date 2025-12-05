/*****************************************************************************
 * @file invoice_data_test.cpp
 *
 * @brief
 *   Unit tests for the data::invoice model, covering invoice metadata,
 *   monetary totals, and column-based line-item data.
 *
 * @details
 *   This test suite validates the behavior of the invoice data structure,
 *   which extends the common billing model with additional invoice-specific
 *   fields and collections of detail lines. The following areas are covered:
 *
 *   - Field assignment and retrieval:
 *       • Job card number
 *       • Order number
 *       • Description total
 *       • Material total
 *       • Grand total
 *
 *   - Column collections:
 *       • Populating description-column line items
 *       • Populating material-column line items
 *       • Verifying that column vectors are stored and returned correctly
 *
 *   - Validation logic:
 *       • Ensures that required invoice fields are non-empty
 *       • Enforces length constraints on textual invoice fields
 *       • Confirms that invoices with missing or oversized data are rejected
 *
 *   - Copy and move semantics:
 *       • Copy-assignment and copy-construction
 *       • Move-assignment and move-construction
 *       • Ensures that validity is preserved after ownership transfer
 *
 *   - Boundary and error scenarios:
 *       • Tests behavior when fields exceed defined limits
 *       • Verifies that completely uninitialized or empty invoices fail
 *         validation as expected.
 *
 *   Collectively, these tests help ensure that data::invoice behaves as a
 *   robust and reliable representation of invoice entities in the system.
 *****************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <iostream>
#include <vector>
#include <invoice_data.h>
#include <column_data.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Assign the job card number. (Done)
 * 2) Retrieve the job card number. (Done)
 * 3) Assign the order number. (Done)
 * 4) Retrieve the order number. (Done)
 * 5) Collect unlimited data in description column view. (Done)
 * 6) Collect unlimited data in the material used column view. (Done)
 * 7) Ensure none of the fields are empty. (Done)
 * 8) All fields should be contraint to 50 characters except the description
 *    column field in both the column views. (Done)
 * 9) Ensure the data is copyable. (Done)
 * 10) Ensure the data is moveable. (Done)
 * 11) Ensure the data is thread safe. (Done)
 * 12) Assign the total description data. (Done)
 * 13) Retrieve the total description data. (Done)
 * 14) Assign the total material data. (Done)
 * 15) Retrieve the total material data. (Done)
 * 16) Assign the grand total data. (Done)
 * 17) Retrieve the grand total data. (Done)
 ******************************************************************************/
TEST_GROUP(invoice_data_test)
{
        data::invoice invoice_data;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(invoice_data_test, handle_the_invoice_job_card)
{
        std::string expected{"24/md"};
        invoice_data.set_job_card_number(expected);
        std::string result{invoice_data.get_job_card_number()};

        STRCMP_EQUAL(expected.c_str(), result.c_str());
}

TEST(invoice_data_test, handle_the_invoice_order_number)
{
        std::string expected{"order number 123"};
        invoice_data.set_order_number(expected);
        std::string result{invoice_data.get_order_number()};

        STRCMP_EQUAL(expected.c_str(), result.c_str());
}

TEST(invoice_data_test, handle_description_total_data)
{
        std::string expected{"5999.90"};
        invoice_data.set_description_total(expected);
        std::string result{invoice_data.get_description_total()};

        STRCMP_EQUAL(expected.c_str(), result.c_str());
}

TEST(invoice_data_test, handle_material_total_data)
{
        std::string expected{"5999.90"};
        invoice_data.set_material_total(expected);
        std::string result{invoice_data.get_material_total()};

        STRCMP_EQUAL(expected.c_str(), result.c_str());
}

TEST(invoice_data_test, handle_grand_total_data)
{
        std::string expected{"5999.90"};
        invoice_data.set_grand_total(expected);
        std::string result{invoice_data.get_grand_total()};

        STRCMP_EQUAL(expected.c_str(), result.c_str());
}

TEST(invoice_data_test, handle_description_column_data)
{
        const int size{50};
        std::vector<data::column> vec{};
        for (unsigned int i = 0; i < size; ++i)
        {
                data::column expected{};
                expected.set_quantity(i);
                expected.set_description("machining");
                expected.set_amount(55 + i + .0);
                vec.push_back(expected);
        }
        invoice_data.set_description_column(vec);
        std::vector<data::column> result = invoice_data.get_description_column();

        CHECK_EQUAL(size, result.size());
}

TEST(invoice_data_test, handle_material_column_data)
{
        const int size{50};
        std::vector<data::column> vec{};
        for (unsigned int i = 0; i < size; ++i)
        {
                data::column expected{};
                expected.set_quantity(i);
                expected.set_description("machining");
                expected.set_amount(55 + i + .0);
                vec.push_back(expected);
        }
        invoice_data.set_material_column(vec);
        std::vector<data::column> result = invoice_data.get_material_column();

        CHECK_EQUAL(size, result.size());
}

TEST(invoice_data_test, handle_the_copy_of_the_data)
{
        const int size{50};
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
        std::string paid_status{"Not Paid"};
        std::string name{"Test Business"};
        std::string description_total{"1234.00"};
        std::string material_total{"1234.00"};
        std::string grand_total{"1234.00"};
        std::string number{"1"};

        invoice_data.set_name(name);
        invoice_data.set_id(number);
        invoice_data.set_date(date);
        invoice_data.set_paid_status(paid_status);
        invoice_data.set_job_card_number(card_number);
        invoice_data.set_order_number(order_number);
        invoice_data.set_description_total(description_total);
        invoice_data.set_material_total(material_total);
        invoice_data.set_grand_total(grand_total);
        invoice_data.set_material_column(vec);
        invoice_data.set_description_column(vec);
        data::invoice result;
        result = invoice_data;

        CHECK_EQUAL(true, result.is_valid());
}

TEST(invoice_data_test, handle_the_move_of_the_data)
{
        const int size{50};
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
        std::string paid_status{"Not Paid"};
        std::string name{"Test Business"};
        std::string description_total{"1234.00"};
        std::string material_total{"1234.00"};
        std::string grand_total{"1234.00"};
        std::string number{"1"};

        invoice_data.set_name(name);
        invoice_data.set_id(number);
        invoice_data.set_date(date);
        invoice_data.set_paid_status(paid_status);
        invoice_data.set_job_card_number(card_number);
        invoice_data.set_order_number(order_number);
        invoice_data.set_description_total(description_total);
        invoice_data.set_material_total(material_total);
        invoice_data.set_grand_total(grand_total);
        invoice_data.set_material_column(vec);
        invoice_data.set_description_column(vec);
        data::invoice result{};
        result = std::move(invoice_data);

        CHECK_EQUAL(true, result.is_valid());
}

TEST(invoice_data_test, handle_copy_constructing_of_the_data)
{
        const int size{50};
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
        std::string paid_status{"Not Paid"};
        std::string name{"Test Business"};
        std::string description_total{"1234.00"};
        std::string material_total{"1234.00"};
        std::string grand_total{"1234.00"};
        std::string number{"1"};

        invoice_data.set_name(name);
        invoice_data.set_id(number);
        invoice_data.set_date(date);
        invoice_data.set_paid_status(paid_status);
        invoice_data.set_job_card_number(card_number);
        invoice_data.set_order_number(order_number);
        invoice_data.set_description_total(description_total);
        invoice_data.set_material_total(material_total);
        invoice_data.set_grand_total(grand_total);
        invoice_data.set_material_column(vec);
        invoice_data.set_description_column(vec);
        data::invoice result{invoice_data};

        CHECK_EQUAL(true, result.is_valid());
}

TEST(invoice_data_test, handle_move_constructing_of_the_data)
{
        const int size{50};
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
        std::string paid_status{"Not Paid"};
        std::string name{"Test Business"};
        std::string description_total{"1234.00"};
        std::string material_total{"1234.00"};
        std::string grand_total{"1234.00"};
        std::string number{"1"};

        invoice_data.set_name(name);
        invoice_data.set_id(number);
        invoice_data.set_date(date);
        invoice_data.set_paid_status(paid_status);
        invoice_data.set_job_card_number(card_number);
        invoice_data.set_order_number(order_number);
        invoice_data.set_description_total(description_total);
        invoice_data.set_material_total(material_total);
        invoice_data.set_grand_total(grand_total);
        invoice_data.set_material_column(vec);
        invoice_data.set_description_column(vec);
        data::invoice result{std::move(invoice_data)};

        CHECK_EQUAL(true, result.is_valid());
}

TEST(invoice_data_test, handle_failure_to_set_data)
{
        std::vector<data::column> vec{};
        std::string order_number{""};
        std::string card_number{""};
        std::string date{""};
        std::string paid_status{""};
        std::string name{""};
        std::string description_total{""};
        std::string material_total{""};
        std::string grand_total{""};
        std::string number{""};

        invoice_data.set_name(name);
        invoice_data.set_id(number);
        invoice_data.set_date(date);
        invoice_data.set_paid_status(paid_status);
        invoice_data.set_job_card_number(card_number);
        invoice_data.set_order_number(order_number);
        invoice_data.set_description_total(description_total);
        invoice_data.set_material_total(material_total);
        invoice_data.set_grand_total(grand_total);
        invoice_data.set_material_column(vec);
        invoice_data.set_description_column(vec);

        CHECK_EQUAL(false, invoice_data.is_valid());
}

TEST(invoice_data_test, handle_data_limits)
{
        std::string limit{"jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj"};
        const int size{50};
        std::vector<data::column> vec{};
        for (unsigned int i = 0; i < size; ++i)
        {
                data::column expected{};
                expected.set_quantity(i);
                expected.set_description("machining");
                expected.set_amount(55 + i + .0);
                vec.push_back(expected);
        }
        std::string order_number{limit};
        std::string card_number{limit};
        std::string date{limit};
        std::string paid_status{limit};
        std::string name{limit};
        std::string description_total{limit};
        std::string material_total{limit};
        std::string grand_total{limit};
        std::string number{"123456789"};

        invoice_data.set_name(name);
        invoice_data.set_id(number);
        invoice_data.set_date(date);
        invoice_data.set_paid_status(paid_status);
        invoice_data.set_job_card_number(card_number);
        invoice_data.set_order_number(order_number);
        invoice_data.set_description_total(description_total);
        invoice_data.set_material_total(material_total);
        invoice_data.set_grand_total(grand_total);
        invoice_data.set_material_column(vec);
        invoice_data.set_description_column(vec);

        CHECK_EQUAL(false, invoice_data.is_valid());
}

TEST(invoice_data_test, handle_bad_data)
{
        std::vector<data::column> vec{};
        std::string order_number{};
        std::string card_number{};
        std::string date{};
        std::string paid_status{};
        std::string name{};
        std::string description_total{};
        std::string material_total{};
        std::string grand_total{};
        std::string number{};

        invoice_data.set_name(name);
        invoice_data.set_id(number);
        invoice_data.set_date(date);
        invoice_data.set_paid_status(paid_status);
        invoice_data.set_job_card_number(card_number);
        invoice_data.set_order_number(order_number);
        invoice_data.set_description_total(description_total);
        invoice_data.set_material_total(material_total);
        invoice_data.set_grand_total(grand_total);
        invoice_data.set_material_column(vec);
        invoice_data.set_description_column(vec);

        CHECK_EQUAL(false, invoice_data.is_valid());
}
