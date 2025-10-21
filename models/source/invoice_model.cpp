/********************************************************
 * Contents: Client invoice interface implementation
 * Author: Dawid J. Blom
 * Date: December 9, 2024
 *
 * NOTE:
 *******************************************************/
#include <invoice_model.h>
#include <invoice_pdf.h>
#include <future>
#include <syslog.h>
#include <sqlite.h>
#include <admin_data.h>
#include <client_data.h>
#include <invoice_data.h>
#include <admin_serialize.h>
#include <client_serialize.h>
#include <invoice_serialize.h>


model::invoice::invoice(const std::string& _database_file, const std::string& _database_password)
	: database_file{_database_file}, database_password{_database_password} {}

model::invoice::~invoice() {}

std::vector<std::any> model::invoice::load(const std::string& _business_name) const
{
	std::vector<std::any> pdf_invoices_data{};
        if (_business_name.empty())
	{
		syslog(LOG_CRIT, "INVOICE_MODEL: invalid argument - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
        {
		storage::database::sqlite database{this->database_file, this->database_password};
		serialize::admin admin_serialize{};
		data::admin admin_data{
			std::any_cast<data::admin>(
				admin_serialize.extract_data(
					database.select(sql::query::invoice_admin_select)
				)
			)
		};

		storage::database::sql_parameters client_params = {_business_name};
		serialize::client client_serialize{};
		data::client client_data{
			std::any_cast<data::client>(
				client_serialize.extract_data(
					database.select(sql::query::invoice_client_select, client_params)
				)
			)
		};

		serialize::column column_serialize{};
		serialize::invoice invoice_serialize{};
		storage::database::sql_parameters invoice_params = {_business_name};
		for (const std::any& data : invoice_serialize.extract_data(
				database.select(sql::query::invoice_select, invoice_params)))
		{
			data::invoice invoice_data{std::any_cast<data::invoice> (data)};
			storage::database::sql_parameters column_params = {std::stoi(invoice_data.get_invoice_number())};
			std::vector<data::column> material_column_data{column_serialize.extract_data(
						database.select(sql::query::material_labor_select, column_params)
					)};
			std::vector<data::column> description_column_data{column_serialize.extract_data(
						database.select(sql::query::description_labor_select, column_params)
					)};

			invoice_data.set_material_column(material_column_data);
			invoice_data.set_description_column(description_column_data);

			data::pdf_invoice pdf_invoice_data{};
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
	data::invoice invoice_data{std::any_cast<data::invoice> (_data)};
        if (invoice_data.is_valid() == false)
	{
		syslog(LOG_CRIT, "INVOICE_MODEL: invalid argument - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
        {
		serialize::invoice invoice_serialize{};
		storage::database::sql_parameters invoice_params{invoice_serialize.package_data(invoice_data)};
		storage::database::sqlite database{this->database_file, this->database_password};
		if (database.transaction("BEGIN IMMEDIATE;") == false)
		{
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "INVOICE_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else if (database.usert(sql::query::invoice_usert, invoice_params) == false)
		{
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "INVOICE_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}

		serialize::column column_serialize{};
		for (const data::column& column_data : invoice_data.get_description_column())
		{
			storage::database::sql_parameters description_params{column_serialize.package_data(column_data, invoice_data)};
			if (database.usert(sql::query::labor_usert, description_params) == false)
			{
				if (database.transaction("ROLLBACK;") == false)
				{
					syslog(LOG_CRIT, "INVOICE_MODEL: failed to rollback - "
							 "filename %s, line number %d", __FILE__, __LINE__);
				}
			}
		}

		for (const data::column& column_data : invoice_data.get_material_column())
		{
			storage::database::sql_parameters material_params{column_serialize.package_data(column_data, invoice_data)};
			if (database.usert(sql::query::labor_usert, material_params) == false)
			{
				if (database.transaction("ROLLBACK;") == false)
				{
					syslog(LOG_CRIT, "INVOICE_MODEL: failed to rollback - "
							 "filename %s, line number %d", __FILE__, __LINE__);
				}
			}
		}

		if (database.transaction("COMMIT;") == false)
		{
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
