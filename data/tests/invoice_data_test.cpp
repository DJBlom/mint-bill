/*******************************************************************************
 * Contents: invoice data unit tests
 * Author: Dawid Blom
 * Date: December 4, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
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
 * 1) Assign the business name searched for. (Done)
 * 2) Retrieve the business name searched for. (Done)
 * 3) Assign the invoice number. (Done)
 * 4) Retrieve the invoice number. (Done)
 * 5) Assign the date. (Done)
 * 6) Retrieve the date. (Done)
 * 7) Assign the job card number. (Done)
 * 8) Retrieve the job card number. (Done)
 * 9) Assign the order number. (Done)
 * 10) Retrieve the order number. (Done)
 * 11) Collect unlimited data in description column view. (Done)
 * 12) Collect unlimited data in the material used column view. (Done)
 * 13) Ensure none of the fields are empty. (Done)
 * 14) All fields should be contraint to 50 characters except the description
 *     column field in both the column views. (Done)
 * 15) Ensure the data is copyable. (Done)
 * 16) Ensure the data is moveable. (Done)
 * 17) Ensure the data is thread safe. (Done)
 * 18) Assign the total description data. (Done)
 * 19) Retrieve the total description data. (Done)
 * 20) Assign the total material data. (Done)
 * 21) Retrieve the total material data. (Done)
 * 22) Assign the grand total data. (Done)
 * 23) Retrieve the grand total data. (Done)
 * 24) Assign and retrieve the paid status.
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

TEST(invoice_data_test, handle_the_business_name)
{
        std::string expected{"Test Business"};
        invoice_data.set_business_name(expected);
        std::string result{invoice_data.get_business_name()};

        STRCMP_EQUAL(expected.c_str(), result.c_str());
}

TEST(invoice_data_test, handle_the_invoice_number)
{
        std::string expected{"1"};
        invoice_data.set_invoice_number(expected);
        std::string result{invoice_data.get_invoice_number()};

        CHECK_EQUAL(expected, result);
}

TEST(invoice_data_test, handle_the_invoice_date)
{
        std::string expected{"2023-09-04"};
        invoice_data.set_invoice_date(expected);
        std::string result{invoice_data.get_invoice_date()};

        STRCMP_EQUAL(expected.c_str(), result.c_str());
}

TEST(invoice_data_test, handle_the_invoice_paid_status)
{
        std::string expected{"Not Paid"};
        invoice_data.set_paid_status(expected);
        std::string result{invoice_data.get_paid_status()};

        STRCMP_EQUAL(expected.c_str(), result.c_str());
}

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

        invoice_data.set_business_name(name);
        invoice_data.set_invoice_number(number);
        invoice_data.set_invoice_date(date);
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

        invoice_data.set_business_name(name);
        invoice_data.set_invoice_number(number);
        invoice_data.set_invoice_date(date);
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

        invoice_data.set_business_name(name);
        invoice_data.set_invoice_number(number);
        invoice_data.set_invoice_date(date);
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

        invoice_data.set_business_name(name);
        invoice_data.set_invoice_number(number);
        invoice_data.set_invoice_date(date);
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

        invoice_data.set_business_name(name);
        invoice_data.set_invoice_number(number);
        invoice_data.set_invoice_date(date);
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

        invoice_data.set_business_name(name);
        invoice_data.set_invoice_number(number);
        invoice_data.set_invoice_date(date);
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

        invoice_data.set_business_name(name);
        invoice_data.set_invoice_number(number);
        invoice_data.set_invoice_date(date);
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
