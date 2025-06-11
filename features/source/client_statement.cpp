/********************************************************
 * Contents: Client statement definition
 * Author: Dawid J. Blom
 * Date: June 11, 2025
 *
 * NOTE:
 *******************************************************/
#include <statement_data.h>
#include <client_statement.h>

feature::client_statement::~client_statement() {}

std::any feature::client_statement::load(const std::string& _business_name, const interface::storage& _db)
{
	(void) _business_name;
	(void) _db;
	data::statement statement_data{};
        statement_data.set_invoice_number("1234");
        statement_data.set_date("2025/06/14");
        statement_data.set_order_number("md 1234");
        statement_data.set_paid_status("Yes");
        statement_data.set_price("R1234.00");
	std::any temp{statement_data};

	return temp;
}

bool feature::client_statement::save(const std::any& _data, const interface::storage& _db)
{
	(void) _data;
	(void) _db;

	return false;
}

std::any feature::client_statement::search(const std::string& _business_name, const interface::storage& _db)
{
	(void) _business_name;
	(void) _db;
	std::any temp;

	return temp;
}
