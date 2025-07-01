/*******************************************************
 * Contents: Statement Page class declaration
 * Author: Dawid J. Blom
 * Date: February 19, 2025
 *
 * NOTE:
 *******************************************************/
#include <statement_page.h>
#include <client_invoice.h>
#include <client_statement.h>
#include <pdf_statement_data.h>

#include <iostream>

gui::statement_page::~statement_page() {}

bool gui::statement_page::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool created{false};
        if (_ui_builder)
        {
                // save
                if (this->save_dialog.create(_ui_builder) == false)
                {
                        return created;
                }

                if (this->save_dialog.connect() == false)
                {
                        return created;
                }

                // print
                if (this->print_dialog.create(_ui_builder) == false)
                {
                        return created;
                }

                if (this->print_dialog.connect() == false)
                {
                        return created;
                }

                if (this->email_dialog.create(_ui_builder) == false)
                {
                        return created;
                }

                if (this->email_dialog.connect() == false)
                {
                        return created;
                }

		if (this->statement_view.create(_ui_builder) == false)
		{
                        return created;
		}

		(void) this->date.create("Date");
		(void) this->price.create("Price");
		(void) this->paid_status.create("Paid Status");
		(void) this->order_number.create("Order Number");
		(void) this->invoice_number.create("Invoice Number");

		if (this->statement_view.add_column(invoice_number) == false)
		{
                        return created;
		}
		if (this->statement_view.add_column(date) == false)
		{
                        return created;
		}
		if (this->statement_view.add_column(order_number) == false)
		{
                        return created;
		}
		if (this->statement_view.add_column(paid_status) == false)
		{
                        return created;
		}
		if (this->statement_view.add_column(price) == false)
		{
                        return created;
		}

		if (this->invoice_pdf_view.create(_ui_builder) == false)
		{
                        return created;
		}

		created = true;
        }

        return created;
}

bool gui::statement_page::search(const std::string& _keyword)
{
        bool searched{true};
        if (_keyword.empty())
        {
                syslog(LOG_CRIT, "The _keywword is empty - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		searched = false;
        }
        else
        {
		feature::client_statement client_statement{};
		std::vector<std::any> invoices{};
		std::vector<std::any> statements{};
		for (const std::any& data : client_statement.load(_keyword))
		{
			data::pdf_statement pdf_statement{std::any_cast<data::pdf_statement>(data)};
			statements.emplace_back(pdf_statement.get_statement());
			const std::vector<data::pdf_invoice>& pdf_invoices = pdf_statement.get_pdf_invoices();
			std::transform(pdf_invoices.begin(), pdf_invoices.end(), std::back_inserter(invoices),
				[](const data::pdf_invoice& invoice) {
				return std::any{invoice};
			});
		}

		if (this->statement_view.populate(statements) == false)
		{
			searched = false;
		}

		if (this->invoice_pdf_view.populate(invoices) == false)
		{
			searched = false;
		}
        }

        return searched;
}

bool gui::statement_page::print()
{
	return false;
}

bool gui::statement_page::email()
{
	return false;
}

bool gui::statement_page::save()
{
	bool success{false};
	if (this->save_dialog.is_not_valid())
	{
                syslog(LOG_CRIT, "The save_alert_dialog is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->save_dialog.show();
	}

	return success;
}
