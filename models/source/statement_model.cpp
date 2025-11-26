/********************************************************
 * Contents: Client statement definition
 * Author: Dawid J. Blom
 * Date: June 11, 2025
 *
 * NOTE:
 *******************************************************/
#include <future>
#include <algorithm>
#include <statement_pdf.h>
#include <statement_model.h>
#include <pdf_invoice_data.h>
#include <pdf_statement_data.h>


#include <iostream>

model::statement::~statement() {}

std::vector<std::any> model::statement::load(const std::string& _business_name) const
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
			client_data.set_name(_business_name);
			client_data.set_address("Geelsterd 8");
			client_data.set_area_code("543543");
			client_data.set_town("George");
			client_data.set_cellphone("0832315944");
			client_data.set_email("dmnsstmtest@gmail.com dawidjblom@gmail.com");
			client_data.set_vat_number("3241324321413");
			client_data.set_statement_schedule("4,4");
			pdf_invoice_data.set_client(client_data);

			data::admin business_data;
			business_data.set_name("T.M Engineering");
			business_data.set_address("geelsterd 8");
			business_data.set_area_code("5432");
			business_data.set_town("george");
			business_data.set_cellphone("0832315944");
			business_data.set_email("dmnsstmtest@gmail.com");
			business_data.set_bank("Standard Bank");
			business_data.set_branch_code("043232");
			business_data.set_account_number("0932443824");
			business_data.set_client_message("Thank you for your support");
			business_data.set_password("bxwx eaku ndjj ltda");
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
					invoice_column_data.set_amount(55554 + k + .0);
					vec.push_back(invoice_column_data);
				}

				data::invoice invoice_data;
				invoice_data.set_business_name(_business_name);
				invoice_data.set_invoice_number(std::to_string(j));
				invoice_data.set_invoice_date("2023-09-04");
				invoice_data.set_paid_status("Paid");
				invoice_data.set_job_card_number("24/md");
				invoice_data.set_order_number("order 123");
				invoice_data.set_description_total("1235.00");
				invoice_data.set_material_total("1237.00");

				std::ostringstream price{""};
				price << std::fixed << std::setprecision(2) << vec[j].get_amount();
				invoice_data.set_grand_total(price.str());
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

bool model::statement::save(const std::any& _data) const
{
	(void)_data;

	return false;
}

data::email model::statement::prepare_for_email(const std::vector<std::any>& _pdf_statements) const
{
	data::email email_data;
	for (const std::any& _pdf_statement : _pdf_statements)
	{
		data::pdf_statement pdf_statement_data{std::any_cast<data::pdf_statement> (_pdf_statement)};
		for (const data::pdf_invoice& pdf_invoice : pdf_statement_data.get_pdf_invoices())
		{
			email_data.set_client(pdf_invoice.get_client());
			email_data.set_business(pdf_invoice.get_business());
			break;
		}
		break;
	}
	email_data.set_subject("Statement");
	email_data.set_attachments(this->convert_pdfs_to_strings(_pdf_statements));
	if (email_data.is_valid())
	{
		std::cout << "Email data is valid\n";
	}

	return email_data;
}

std::vector<std::string> model::statement::prepare_for_print(const std::vector<std::any>& _pdf_statements) const
{
	return this->convert_pdfs_to_strings(_pdf_statements);
}

std::vector<std::string> model::statement::convert_pdfs_to_strings(const std::vector<std::any>& _pdf_statements) const
{
	std::vector<std::future<std::string>> pdf_documents;
	std::transform(_pdf_statements.cbegin(),
			_pdf_statements.cend(),
			std::back_inserter(pdf_documents),
			[] (const std::any& _pdf_statement) {
				return std::async(std::launch::async, [&_pdf_statement] {
					feature::statement_pdf pdf{};
					return pdf.generate(_pdf_statement);
				});
			});


	std::vector<std::string> pdfs;
	std::transform(
		std::make_move_iterator(pdf_documents.begin()),
		std::make_move_iterator(pdf_documents.end()),
		std::back_inserter(pdfs),
		[](std::future<std::string> _pdf) {
			return _pdf.get();
		});

	return pdfs;
}
