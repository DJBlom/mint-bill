/********************************************************
 * Contents: Client invoice interface implementation
 * Author: Dawid J. Blom
 * Date: December 9, 2024
 *
 * NOTE:
 *******************************************************/
#include <invoice_model.h>
#include <client_data.h>
#include <admin_data.h>
#include <invoice_pdf.h>
#include <syslog.h>
#include <sqlite.h>
#include <future>
#include <invoice_data.h>


#include <iostream>


namespace sql {
namespace query {
	constexpr const char* invoice_usert{R"sql(
		INSERT INTO invoice (
			business_id,
			order_number,
			job_card_number,
			date_created,
			paid_status,
			material_total,
			description_total,
			grand_total
		)
		VALUES (
			(SELECT c.business_id
				FROM client c
				JOIN business_details b ON b.business_id = c.business_id
				WHERE b.business_name = ?),
			?, ?, ?, ?, ?, ?, ?
		)
		ON CONFLICT (business_id, order_number, job_card_number) DO UPDATE SET
			date_created      = excluded.date_created,
			paid_status       = excluded.paid_status,
			material_total    = excluded.material_total,
			description_total = excluded.description_total,
			grand_total       = excluded.grand_total;
	)sql"};

	constexpr const char* labor_usert{R"sql(
		INSERT INTO labor (
			invoice_id,
			line_number,
			is_description,
			quantity,
			description,
			amount
		)
		VALUES (
			(SELECT i.invoice_id
				FROM invoice i
				JOIN business_details b ON b.business_id = i.business_id
				WHERE b.business_name   = ?
				AND i.order_number    = ?
				AND i.job_card_number = ?),
			?, ?, ?, ?, ?
		)
		ON CONFLICT (invoice_id, line_number, is_description) DO UPDATE SET
			quantity    = excluded.quantity,
			description = excluded.description,
			amount      = excluded.amount;
	)sql"};

	constexpr const char* admin_select{R"sql(
		SELECT
			bd.business_name,
			bd.email_address,
			bd.contact_number,
			bd.street, bd.area_code, bd.town_name,
			a.bank_name, a.branch_code, a.account_number, a.app_password, a.client_message
		FROM admin a
		JOIN business_details bd ON bd.business_id = a.business_id;
	)sql"};

	constexpr const char* client_select{R"sql(
		SELECT
			b.business_name,
			b.email_address,
			b.contact_number,
			b.street,
			b.area_code,
			b.town_name,
			c.vat_number,
			c.statement_schedule
		FROM client c
		JOIN business_details b ON b.business_id = c.business_id
		WHERE b.business_name = ?;
	)sql"};

	constexpr const char* invoice_select{R"sql(
		SELECT
			b.business_name,
			i.invoice_id,
			i.order_number,
			i.job_card_number,
			i.date_created,
			i.paid_status,
			i.material_total,
			i.description_total,
			i.grand_total
		FROM invoice i
		JOIN business_details b ON b.business_id = i.business_id
		WHERE b.business_name = ?;
	)sql"};

	constexpr const char* labor_select{R"sql(
		SELECT
			amount,
			description ,
			quantity,
			is_description
		FROM labor
		WHERE invoice_id = ?
		ORDER BY is_description, line_number;
	)sql"};
}
}

model::invoice::invoice(const std::string& _database_file, const std::string& _database_password)
	: database_file{_database_file}, database_password{_database_password}
{
}

model::invoice::~invoice() {}

std::vector<std::any> model::invoice::load(const std::string& _business_name) const
{
	std::vector<std::any> pdf_invoices_data{};
        if (!_business_name.empty())
        {
		storage::database::sqlite database{this->database_file, this->database_password};
		data::admin admin_data;
		std::vector<std::string> admin_sql_data{};
		for (const storage::database::part::row& row : database.select(sql::query::admin_select))
		{
			for (const storage::database::part::column_value& column_value : row)
			{
				admin_sql_data.emplace_back(std::visit([] (auto&& arg) -> std::string {
					using T = std::decay_t<decltype(arg)>;
					std::string result{""};
					if constexpr (std::is_same_v<T, std::string>)
					{
						result = arg;
					}
					return result;
				}, column_value));
			}
		}

		admin_data.set_name(admin_sql_data[0]);
		admin_data.set_email(admin_sql_data[1]);
		admin_data.set_cellphone(admin_sql_data[2]);
		admin_data.set_address(admin_sql_data[3]);
		admin_data.set_area_code(admin_sql_data[4]);
		admin_data.set_town(admin_sql_data[5]);
		admin_data.set_bank(admin_sql_data[6]);
		admin_data.set_branch_code(admin_sql_data[7]);
		admin_data.set_account_number(admin_sql_data[8]);
		admin_data.set_password(admin_sql_data[9]);
		admin_data.set_client_message(admin_sql_data[10]);


		data::client client_data{};
		std::vector<storage::database::param_values> client_argument = {_business_name};
		std::vector<std::string> client_sql_data{};
		for (const storage::database::part::row& row : database.select(sql::query::client_select, client_argument))
		{
			for (const storage::database::part::column_value& column_value : row)
			{
				client_sql_data.emplace_back(std::visit([] (auto&& arg) -> std::string {
					using T = std::decay_t<decltype(arg)>;
					std::string result{""};
					if constexpr (std::is_same_v<T, std::string>)
					{
						result = arg;
					}
					return result;
				}, column_value));
			}
		}

		client_data.set_business_name(client_sql_data[0]);
		client_data.set_email(client_sql_data[1]);
		client_data.set_business_address(client_sql_data[2]);
		client_data.set_business_area_code(client_sql_data[3]);
		client_data.set_business_town_name(client_sql_data[4]);
		client_data.set_cellphone_number(client_sql_data[5]);
		client_data.set_vat_number(client_sql_data[6]);
		client_data.set_statement_schedule(client_sql_data[7]);


		std::vector<storage::database::param_values> invoice_argument = {_business_name};
		for (const storage::database::part::row& row : database.select(sql::query::invoice_select, invoice_argument))
		{
			data::invoice invoice_data{};
			data::pdf_invoice pdf_invoice_data{};
			std::vector<std::string> invoice_sql_data{};
			for (const storage::database::part::column_value& column_value : row)
			{
				invoice_sql_data.emplace_back(std::visit([] (auto&& arg) -> std::string {
					using T = std::decay_t<decltype(arg)>;
					std::string result{""};
					if constexpr (std::is_same_v<T, std::string>)
					{
						std::cout << "Invoice data: " << arg << std::endl;
						result = arg;
					}
					else if constexpr (std::is_same_v<T, sqlite3_int64>)
					{
						std::cout << "Invoice number: " << arg << std::endl;
						result = std::to_string(arg);
					}

					return result;
				}, column_value));
			}

			invoice_data.set_business_name(invoice_sql_data[0]);
			invoice_data.set_invoice_number(invoice_sql_data[1]);
			invoice_data.set_order_number(invoice_sql_data[2]);
			invoice_data.set_job_card_number(invoice_sql_data[3]);
			invoice_data.set_invoice_date(invoice_sql_data[4]);
			invoice_data.set_paid_status(invoice_sql_data[5]);
			invoice_data.set_material_total(invoice_sql_data[6]);
			invoice_data.set_description_total(invoice_sql_data[7]);
			invoice_data.set_grand_total(invoice_sql_data[8]);

			bool is_description_flag = false;
			std::vector<data::column> labor_material{};
			std::vector<data::column> labor_description{};
			std::vector<storage::database::param_values> labor_argument = {std::stoi(invoice_data.get_invoice_number())};
			for (const storage::database::part::row& row : database.select(sql::query::labor_select, labor_argument))
			{
				int col_index{0};
				data::column data{};
				for (const storage::database::part::column_value& column_value : row)
				{
					std::visit([&](auto&& arg) {
						using T = std::decay_t<decltype(arg)>;
						if (col_index == 0)
						{
							if constexpr (std::is_same_v<T, double>)
								data.set_amount(arg);
						}
						else if (col_index == 1)
						{
							if constexpr (std::is_same_v<T, std::string>)
								data.set_description(arg);
						}
						else if (col_index == 2)
						{
							if constexpr (std::is_same_v<T, sqlite3_int64>)
								data.set_quantity(static_cast<int> (arg));
						}
						else if (col_index == 3)
						{
							if constexpr (std::is_same_v<T, sqlite3_int64>)
								is_description_flag = (static_cast<int>(arg) != 0);
						}
					}, column_value);
					++col_index;
				}

				if (is_description_flag)
				{
					if (data.is_valid())
					{
						labor_description.emplace_back(std::move(data));
					}
				}
				else
				{
					if (data.is_valid())
					{
						labor_material.emplace_back(std::move(data));
					}
				}
			}

			invoice_data.set_description_column(labor_description);
			invoice_data.set_material_column(labor_material);

			pdf_invoice_data.set_invoice(invoice_data);
			pdf_invoice_data.set_client(client_data);
			pdf_invoice_data.set_business(admin_data);


			pdf_invoices_data.emplace_back(std::move(pdf_invoice_data));
		}
        }

        return pdf_invoices_data;
}

bool model::invoice::save(const std::any& _data) const
{
        bool success{false};
	data::invoice data{std::any_cast<data::invoice> (_data)};
        if (data.is_valid() == false)
	{
		syslog(LOG_CRIT, "INVOICE_MODEL: invalid argument - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
        {
		std::vector<storage::database::param_values> invoice_params = {
			data.get_business_name(),
			data.get_order_number(),
			data.get_job_card_number(),
			data.get_invoice_date(),
			data.get_paid_status(),
			data.get_material_total(),
			data.get_description_total(),
			data.get_grand_total()
		};

		storage::database::sqlite database{this->database_file, this->database_password};
		if (database.transaction("BEGIN IMMEDIATE;") == false)
		{
			syslog(LOG_CRIT, "INVOICE_MODEL: failed to begin transaction - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "INVOICE_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else if (database.usert(sql::query::invoice_usert, invoice_params) == false)
		{
			syslog(LOG_CRIT, "INVOICE_MODEL: failed to execute sql query - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "INVOICE_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}

		int line_number{0};
		for (const data::column& column : data.get_description_column())
		{
			if (column.is_valid() == false)
			{
				syslog(LOG_CRIT, "INVOICE_MODEL: failed to execute sql query - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				std::vector<storage::database::param_values> description_params = {
					data.get_business_name(),
					data.get_order_number(),
					data.get_job_card_number(),
					line_number++,
					1,
					column.get_quantity(),
					column.get_description(),
					column.get_amount()
				};
				if (database.usert(sql::query::labor_usert, description_params) == false)
				{
					syslog(LOG_CRIT, "INVOICE_MODEL: failed to execute sql query - "
							 "filename %s, line number %d", __FILE__, __LINE__);
					if (database.transaction("ROLLBACK;") == false)
					{
						syslog(LOG_CRIT, "INVOICE_MODEL: failed to rollback - "
								 "filename %s, line number %d", __FILE__, __LINE__);
					}
				}
			}
		}

		line_number = 0;
		for (const data::column& column : data.get_description_column())
		{
			if (column.is_valid() == false)
			{
				syslog(LOG_CRIT, "INVOICE_MODEL: failed to execute sql query - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				std::vector<storage::database::param_values> material_params = {
					data.get_business_name(),
					data.get_order_number(),
					data.get_job_card_number(),
					line_number++,
					0,
					column.get_quantity(),
					column.get_description(),
					column.get_amount()
				};
				if (database.usert(sql::query::labor_usert, material_params) == false)
				{
					syslog(LOG_CRIT, "INVOICE_MODEL: failed to execute sql query - "
							 "filename %s, line number %d", __FILE__, __LINE__);
					if (database.transaction("ROLLBACK;") == false)
					{
						syslog(LOG_CRIT, "INVOICE_MODEL: failed to rollback - "
								 "filename %s, line number %d", __FILE__, __LINE__);
					}
				}
			}
		}

		if (database.transaction("COMMIT;") == false)
		{
			syslog(LOG_CRIT, "INVOICE_MODEL: failed to commit transaction - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "INVOICE_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else
		{
			success = true;
		}
                // std::cout << "admin name: " << data.get_business_name() << std::endl;
                // std::cout << "Invoice number: " << data.get_invoice_number() << std::endl;
                // std::cout << "Invoice date: " << data.get_invoice_date() << std::endl;
                // std::cout << "Job card number: " << data.get_job_card_number() << std::endl;
                // std::cout << "Order number: " << data.get_order_number() << std::endl;
                //
                //
                // std::cout << "Description!!!!!!!!!!!!!!!!\n";
                // std::vector<data::column> description_data = data.get_description_column();
                // for (const auto& column : description_data)
                // {
                //         std::cout << "Quantity: " << column.get_quantity() << std::endl;
                //         std::cout << "Description: " << column.get_description() << std::endl;
                //         std::cout << "Amount: " << column.get_amount() << std::endl;
                // }
                //
                // std::cout << "Material!!!!!!!!!!!!!!!!!!!\n";
                // std::vector<data::column> material_data = data.get_material_column();
                // for (const auto& column : material_data)
                // {
                //         std::cout << "Quantity: " << column.get_quantity() << std::endl;
                //         std::cout << "Description: " << column.get_description() << std::endl;
                //         std::cout << "Amount: " << column.get_amount() << std::endl;
                // }
                //
                // std::cout << "Description Total: " << data.get_description_total() << std::endl;
                // std::cout << "Material Total: " << data.get_material_total() << std::endl;
                // std::cout << "Grand Total: " << data.get_grand_total() << std::endl;
        }

        return success;
}

data::email model::invoice::prepare_for_email(const std::vector<std::any>& _pdf_invoices) const
{
	data::email email_data;
	for (const std::any& pdf_invoice : _pdf_invoices)
	{
		data::pdf_invoice pdf_invoice_data{std::any_cast<data::pdf_invoice> (pdf_invoice)};
		email_data.set_client(pdf_invoice_data.get_client());
		email_data.set_business(pdf_invoice_data.get_business());
		break;
	}

	std::vector<std::future<std::string>> pdf_documents;
	std::transform(_pdf_invoices.cbegin(),
			_pdf_invoices.cend(),
			std::back_inserter(pdf_documents),
			[] (const std::any& _pdf_invoice) {
				return std::async(std::launch::async, [&_pdf_invoice] {
					feature::invoice_pdf pdf;
					return pdf.generate(_pdf_invoice);
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


	email_data.set_subject("Invoice");
	email_data.set_attachments(pdfs);
	if (email_data.is_valid())
	{
		std::cout << "Email data is valid\n";
	}

	return email_data;
}

std::vector<std::string> model::invoice::prepare_for_print(const std::vector<std::any>& _pdf_invoice) const
{
	std::vector<std::future<std::string>> pdf_documents;
	std::transform(_pdf_invoice.cbegin(),
			_pdf_invoice.cend(),
			std::back_inserter(pdf_documents),
			[] (const std::any& _pdf_invoice) {
				return std::async(std::launch::async, [&_pdf_invoice] {
					feature::invoice_pdf pdf;
					return pdf.generate(_pdf_invoice);
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
