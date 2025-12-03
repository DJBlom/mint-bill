/********************************************************
 * Contents: Client statement definition
 * Author: Dawid J. Blom
 * Date: June 11, 2025
 *
 * NOTE:
 *******************************************************/
#include <future>
#include <syslog.h>
#include <sqlite.h>
#include <algorithm>
#include <statement_pdf.h>
#include <statement_model.h>
#include <pdf_invoice_data.h>
#include <pdf_statement_data.h>
#include <admin_data.h>
#include <client_data.h>
#include <invoice_data.h>
#include <admin_serialize.h>
#include <client_serialize.h>
#include <invoice_serialize.h>
#include <statement_serialize.h>
#include <date_manager.h>



model::statement::statement(const std::string& _database_file, const std::string& _database_password)
	: database_file{_database_file}, database_password{_database_password} {}

model::statement::~statement() {}

std::vector<std::any> model::statement::load(const std::string& _business_name) const
{
	std::vector<std::any> pdf_statements_data{};
	if (_business_name.empty())
	{
		syslog(LOG_CRIT, "STATEMENT_MODEL: argument not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		storage::database::sqlite database{this->database_file, this->database_password};
		serialize::admin admin_serialize{};
		data::admin admin_data{
			std::any_cast<data::admin>(
				admin_serialize.extract_data(
					database.select(sql::query::admin_select)
				)
			)
		};

		storage::database::sql_parameters client_params = {_business_name};
		serialize::client client_serialize{};
		data::client client_data{
			std::any_cast<data::client>(
				client_serialize.extract_data(
					database.select(sql::query::client_select, client_params)
				)
			)
		};

		serialize::invoice invoice_serialize{};
		serialize::statement statement_serialize{};
		storage::database::sql_parameters params = {_business_name};
		for (const std::any& stmt_sql_data : statement_serialize.extract_data(
				database.select(sql::query::statement_select, params)))
		{
			float total{0.0f};
			data::statement statement_data{std::any_cast<data::statement> (stmt_sql_data)};
			serialize::labor labor_serialize{};
			std::vector<data::pdf_invoice> pdf_invoices_data{};
			storage::database::sql_parameters invoice_params = {_business_name};
			for (const std::any& data : invoice_serialize.extract_data(
					database.select(sql::query::invoice_select, invoice_params)))
			{
				data::invoice invoice_data{std::any_cast<data::invoice> (data)};
				storage::database::sql_parameters column_params = {std::stoi(invoice_data.get_id())};
				std::vector<data::column> material_column_data{labor_serialize.extract_data(
							database.select(sql::query::material_labor_select, column_params)
						)};
				std::vector<data::column> description_column_data{labor_serialize.extract_data(
							database.select(sql::query::description_labor_select, column_params)
						)};

				invoice_data.set_material_column(material_column_data);
				invoice_data.set_description_column(description_column_data);

				data::pdf_invoice pdf_invoice_data{};
				pdf_invoice_data.set_invoice(invoice_data);
				pdf_invoice_data.set_client(client_data);
				pdf_invoice_data.set_business(admin_data);

				total += std::stof(invoice_data.get_grand_total());
				pdf_invoices_data.emplace_back(std::move(pdf_invoice_data));
			}

			std::ostringstream total_ss{""};
			total_ss << std::fixed << std::setprecision(2) << total;

			data::pdf_statement pdf_statement_data{};
			pdf_statement_data.set_number(statement_data.get_id());
			pdf_statement_data.set_date(statement_data.get_date());
			pdf_statement_data.set_total(total_ss.str());
			pdf_statement_data.set_statement(statement_data);
			pdf_statement_data.set_pdf_invoices(pdf_invoices_data);

			pdf_statements_data.push_back(std::move(pdf_statement_data));
		}
	}

	return pdf_statements_data;
}

bool model::statement::save(const std::any& _data) const
{
        bool success{false};
	data::statement statement_data{std::any_cast<data::statement> (_data)};
        if (statement_data.is_valid() == false)
	{
		syslog(LOG_CRIT, "STATEMENT_MODEL: invalid argument - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
        {
		storage::database::sqlite database{this->database_file, this->database_password};
		storage::database::sql_parameters statement_params = {
			statement_data.get_name(),
			statement_data.get_period_start(),
			statement_data.get_period_end(),
			statement_data.get_paid_status()
		};

		if (database.transaction("BEGIN IMMEDIATE;") == false)
		{
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "STATEMENT_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else if (database.usert(sql::query::statement_usert, statement_params) == false)
		{
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "STATEMENT_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else if (database.transaction("COMMIT;") == false)
		{
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "STATEMENT_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else
		{
			success = true;
		}
        }

        return success;
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
