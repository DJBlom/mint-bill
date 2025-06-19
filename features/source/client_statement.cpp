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

std::vector<std::any> feature::client_statement::load(const std::string& _business_name) const
{
	(void) _business_name;
	std::vector<std::any> temp{};
	if (_business_name.empty())
	{

	}
	else
	{
		for (int i = 0; i < 100; ++i)
		{
			data::statement data{};
			data.set_invoice_number("INV-" + std::to_string(i));
			data.set_date("2025/06/14");
			data.set_order_number("MD 1234");
			data.set_paid_status("paid");
			data.set_price("R123" + std::to_string(i) + ".00");
			temp.push_back(data);
		}
	}

	return temp;
}
