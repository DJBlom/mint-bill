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

std::any feature::client_statement::load(const std::string& _business_name) const
{
	(void) _business_name;
	data::statement statement_data{};
        statement_data.set_invoice_number("1234");
        statement_data.set_date("2025/06/14");
        statement_data.set_order_number("md 1234");
        statement_data.set_paid_status("Yes");
        statement_data.set_price("R1234.00");
	std::any temp{statement_data};

	return temp;
}
