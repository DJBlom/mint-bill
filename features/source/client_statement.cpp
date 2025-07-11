/********************************************************
 * Contents: Client statement definition
 * Author: Dawid J. Blom
 * Date: June 11, 2025
 *
 * NOTE:
 *******************************************************/
#include <client_statement.h>
#include <pdf_invoice_data.h>
#include <pdf_statement_data.h>

feature::client_statement::~client_statement() {}

std::vector<std::any> feature::client_statement::load(const std::string& _business_name) const
{
	std::vector<std::any> temp{};
	if (_business_name.empty())
	{

	}
	else
	{
		for (int i = 0; i < 100; ++i)
		{
			data::pdf_invoice pdf_invoice_data{};
			data::client client_data;
			client_data.set_business_name(_business_name);
			client_data.set_business_address("Geelsterd 8");
			client_data.set_business_area_code("543543");
			client_data.set_business_town_name("George");
			client_data.set_cellphone_number("0832315944");
			client_data.set_email("odn@gmail.com");
			client_data.set_vat_number("3241324321413");
			client_data.set_statement_schedule("4,4");
			pdf_invoice_data.set_client(client_data);

			data::business business_data;
			business_data.set_name("T.M Engineering");
			business_data.set_address("geelsterd 8");
			business_data.set_area_code("5432");
			business_data.set_town("george");
			business_data.set_cellphone("0832315944");
			business_data.set_email("odn@gmail.com");
			business_data.set_bank("Standard Bank");
			business_data.set_branch_code("043232");
			business_data.set_account_number("0932443824");
			business_data.set_client_message("Thank you for your support");
			business_data.set_password("");
			pdf_invoice_data.set_business(business_data);

			std::vector<data::pdf_invoice> pdf_invoices{};
			for (int j = 0; j < 50; ++j)
			{
				const int size{50};
				std::vector<data::column> vec{};
				for (unsigned int k = 0; k < size; ++k)
				{
					data::column invoice_column_data{};
					invoice_column_data.set_quantity(k);
					invoice_column_data.set_description("machining");
					invoice_column_data.set_amount(55 + k + .0);
					vec.push_back(invoice_column_data);
				}

				data::invoice invoice_data;
				invoice_data.set_business_name(_business_name);
				invoice_data.set_invoice_number(std::to_string(j));
				invoice_data.set_invoice_date("2023-09-04");
				invoice_data.set_paid_status("Paid");
				invoice_data.set_job_card_number("24/md");
				invoice_data.set_order_number("order 123");
				invoice_data.set_description_total("1234.00");
				invoice_data.set_material_total("1234.00");
				invoice_data.set_grand_total("2468.00");
				invoice_data.set_material_column(vec);
				invoice_data.set_description_column(vec);
				pdf_invoice_data.set_invoice(invoice_data);
				pdf_invoices.push_back(pdf_invoice_data);
			}
			data::pdf_statement pdf_statement_data{};
			pdf_statement_data.set_number(std::to_string(i));
			pdf_statement_data.set_date("02/24/2025");
			pdf_statement_data.set_total("2056.00");
			pdf_statement_data.set_pdf_invoices(pdf_invoices);

			temp.push_back(pdf_statement_data);
		}
	}

	return temp;
}
