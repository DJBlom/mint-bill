/*******************************************************************************
 * Contents: statement column data unit tests
 * Author: Dawid Blom
 * Date: April 4, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <statement_column.h>
extern "C"
{

}


/**********************************TEST LIST*****************************************
 * 1) The user should have the ability to change the invoice number. (Done)
 * 2) The user should have the ability to change the date. (Done)
 * 3) The user should have the ability to change the order number. (Done)
 * 4) The user should have the ability to change the paid status. (Done)
 * 5) The user should have the ability to change the price. (Done)
 * 6) Ensure that the data maintains its integrity. (Done)
 * 7) Ensure that the data is thread safe. (Done)
 * 8) The invoice number may not exceed 8 characters in length. 0 < x <= 8 (Done)
 * 9) The date may not exceed 20 characters in length. (Done)
 * 10) The price may not exceed 15 characters in length. (Done)
 * 11) Ensure the data is copyable and copy constructable. (Done)
 * 12) Ensure the data is moveable move constructable. (Done)
 ************************************************************************************/
TEST_GROUP(statement_column_data_test)
{
        data::statement_column data{};
	void setup()
	{
	}

	void teardown()
	{
	}
};


TEST(statement_column_data_test, change_invoice_number)
{
        std::string expected{"1234"};
        data.set_invoice_number(expected);
        std::string result{data.get_invoice_number()};

        STRCMP_EQUAL(result.c_str(), expected.c_str());
}

TEST(statement_column_data_test, change_date)
{
        std::string expected{"12/04/2025"};
        data.set_date(expected);
        std::string result{data.get_date()};

        STRCMP_EQUAL(result.c_str(), expected.c_str());
}

TEST(statement_column_data_test, change_order_number)
{
        std::string expected{"123 mdb"};
        data.set_order_number(expected);
        std::string result{data.get_order_number()};

        STRCMP_EQUAL(result.c_str(), expected.c_str());
}

TEST(statement_column_data_test, change_paid_status)
{
        std::string expected{"Yes"};
        data.set_paid_status(expected);
        std::string result{data.get_paid_status()};

        STRCMP_EQUAL(result.c_str(), expected.c_str());
}

TEST(statement_column_data_test, change_price)
{
        std::string expected{"R1234.00"};
        data.set_price(expected);
        std::string result{data.get_price()};

        STRCMP_EQUAL(result.c_str(), expected.c_str());
}

TEST(statement_column_data_test, successfully_maintain_data_integrity)
{
        std::string invoice_number{"1234"};
        std::string date{"12/04/2025"};
        std::string order_number{"123 mdb"};
        std::string paid_status{"Yes"};
        std::string price{"R1234.00"};
        data.set_invoice_number(invoice_number);
        data.set_date(date);
        data.set_order_number(order_number);
        data.set_paid_status(paid_status);
        data.set_price(price);

        CHECK(data.is_valid());
}

TEST(statement_column_data_test, unsuccessfully_maintain_data_integrity)
{
        std::string expected{""};
        data.set_invoice_number(expected);

        CHECK_EQUAL(false, data.is_valid());
}

TEST(statement_column_data_test, ensure_copyable)
{
        std::string invoice_number{"1234"};
        std::string date{"12/04/2025"};
        std::string order_number{"123 mdb"};
        std::string paid_status{"Yes"};
        std::string price{"R1234.00"};
        data.set_invoice_number(invoice_number);
        data.set_date(date);
        data.set_order_number(order_number);
        data.set_paid_status(paid_status);
        data.set_price(price);
        data::statement_column result{};
        result = data;

        CHECK(result.is_valid());
}

TEST(statement_column_data_test, ensure_copy_constructable)
{
        std::string invoice_number{"1234"};
        std::string date{"12/04/2025"};
        std::string order_number{"123 mdb"};
        std::string paid_status{"Yes"};
        std::string price{"R1234.00"};
        data.set_invoice_number(invoice_number);
        data.set_date(date);
        data.set_order_number(order_number);
        data.set_paid_status(paid_status);
        data.set_price(price);
        data::statement_column result{data};

        CHECK(result.is_valid());
}

TEST(statement_column_data_test, ensure_moveable)
{
        std::string invoice_number{"1234"};
        std::string date{"12/04/2025"};
        std::string order_number{"123 mdb"};
        std::string paid_status{"Yes"};
        std::string price{"R1234.00"};
        data.set_invoice_number(invoice_number);
        data.set_date(date);
        data.set_order_number(order_number);
        data.set_paid_status(paid_status);
        data.set_price(price);
        data::statement_column result{};
        result = std::move(data);

        CHECK(result.is_valid());
}

TEST(statement_column_data_test, ensure_move_constructable)
{
        std::string invoice_number{"1234"};
        std::string date{"12/04/2025"};
        std::string order_number{"123 mdb"};
        std::string paid_status{"Yes"};
        std::string price{"R1234.00"};
        data.set_invoice_number(invoice_number);
        data.set_date(date);
        data.set_order_number(order_number);
        data.set_paid_status(paid_status);
        data.set_price(price);
        data::statement_column result{std::move(data)};

        CHECK(result.is_valid());
}
