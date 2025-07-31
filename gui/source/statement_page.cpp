/*******************************************************
 * Contents: Statement Page class declaration
 * Author: Dawid J. Blom
 * Date: February 19, 2025
 *
 * NOTE:
 *******************************************************/
#include <statement_page.h>
#include <pdf_statement_data.h>
#include <email.h>
#include <algorithm>
#include <printer.h>

#include <iostream>

gui::statement_page::statement_page()
{
        this->email_dispatcher.connect(sigc::mem_fun(*this, &statement_page::email_sent));
}

gui::statement_page::~statement_page() {}

bool gui::statement_page::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder,
				 const std::shared_ptr<Gtk::Window>& _main_window)
{
	bool created{false};
        if (_ui_builder)
        {
		if (this->no_item_selected.create(_ui_builder) == false)
		{
			return created;
		}

		if (this->email_alert.create(_ui_builder) == false)
		{
			return created;
		}

		if (this->print_alert.create(_ui_builder) == false)
		{
			return created;
		}

		if (this->save_alert.create(_ui_builder) == false)
		{
			return created;
		}

		(void) this->print_alert.connect([this, _main_window] (const int& response) {
			switch (response)
			{
				case GTK_RESPONSE_YES:
					if (this->print_alert.hide() == false)
					{

					}
					else
					{
						std::vector<std::string> data{this->client_statement.prepare_for_print(this->documents)};
						gui::part::printer printer{"statement"};
						(void) printer.print(data, _main_window);
					}
					break;
				case GTK_RESPONSE_NO:
					(void) this->print_alert.hide();
					break;
				default:
					(void) this->print_alert.hide();
					break;
			}
		});

		(void) this->email_alert.connect([this] (const int& response) {
			switch (response)
			{
				case GTK_RESPONSE_YES:
					if (this->email_alert.hide() == false)
					{

					}
					else
					{
						this->email_future = std::move(std::async(std::launch::async, [this] () {
							data::email data{this->client_statement.prepare_for_email(this->documents)};
							feature::email email;
							bool result{email.send(data)};
							this->email_dispatcher.emit();
							return result;
						}));
					}
					break;
				case GTK_RESPONSE_NO:
					(void) this->email_alert.hide();
					break;
				default:
					(void) this->email_alert.hide();
					break;
			}
		});

		(void) this->save_alert.connect([this] (const int& response) {
			switch (response)
			{
				case GTK_RESPONSE_YES:
					(void) this->save_alert.hide();
					for (const std::any& data : this->statement_view.extract())
					{
						data::invoice invoice{std::any_cast<data::invoice> (data)};
						std::cout << "Invoice paid_status: " << invoice.get_paid_status() << std::endl;
					}
					break;
				case GTK_RESPONSE_NO:
					(void) this->save_alert.hide();
					break;
				default:
					(void) this->save_alert.hide();
					break;
			}
		});

		(void) this->no_item_selected.connect([this] (const int& response) {
			switch (response)
			{
				case GTK_RESPONSE_CLOSE:
					(void) this->no_item_selected.hide();
					break;
				default:
					(void) this->no_item_selected.hide();
					break;
			}
		});

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

		if (this->statement_pdf_view.create(_ui_builder) == false)
		{
                        return created;
		}

		(void) this->statement_pdf_view.double_click([this] (const std::any& _data) {
			data::pdf_statement pdf_statement{std::any_cast<data::pdf_statement> (_data)};
			if (pdf_statement.is_valid() == false)
			{
				syslog(LOG_CRIT, "The pdf_statement data is not valid - "
						 "filename %s, line number %d", __FILE__, __LINE__);
				return false;
			}
			else
			{
				std::vector<std::any> invoices{};
				std::vector<std::any> pdf_invoices{};
				for (const data::pdf_invoice& pdf_invoice : pdf_statement.get_pdf_invoices())
				{
					if (pdf_invoice.is_valid() == false)
					{
						syslog(LOG_CRIT, "The pdf_invoice data is not valid - "
								 "filename %s, line number %d", __FILE__, __LINE__);
					}
					else
					{
						invoices.emplace_back(pdf_invoice.get_invoice());
						pdf_invoices.emplace_back(pdf_invoice);
					}
				}
				(void) this->statement_view.populate(invoices);
				(void) this->invoice_pdf_view.populate(pdf_invoices);
			}

			return true;
		});

		(void) this->statement_pdf_view.single_click([this] (const std::vector<std::any>& _pdf_statements) {
			this->documents.clear();
			this->documents = _pdf_statements;
			return true;
		});

		created = true;
        }

        return created;
}

bool gui::statement_page::search(const std::string& _keyword)
{
        bool searched{true};
	if (_keyword.empty())
	{
		searched = false;
		syslog(LOG_CRIT, "The _keyword is empty, clearing all entries - "
				 "filename %s, line number %d", __FILE__, __LINE__);
		this->documents.clear();
		if (this->statement_view.clear() == false)
		{
			syslog(LOG_CRIT, "Could not clear statement_view - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}

		if (this->invoice_pdf_view.clear() == false)
		{
			syslog(LOG_CRIT, "Could not clear invoice_pdf_view - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}

		if (this->statement_pdf_view.clear() == false)
		{
			syslog(LOG_CRIT, "Could not clear statement_pdf_view - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
	}
	else
	{
		std::vector<std::any> pdf_statements{};
		for (const std::any& data : this->client_statement.load(_keyword))
		{
			data::pdf_statement pdf_statement{std::any_cast<data::pdf_statement>(data)};
			pdf_statements.emplace_back(pdf_statement);
		}

		if (this->statement_pdf_view.populate(pdf_statements) == false)
		{
			searched = false;
		}
	}

        return searched;
}

bool gui::statement_page::print()
{
	bool success{false};
	if (this->documents.empty())
	{
		if (this->no_item_selected.show() == false)
		{
			syslog(LOG_CRIT, "Failed to show no_item_selected dialog - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
	}
	else
	{
		if (this->print_alert.show() == false)
		{
			syslog(LOG_CRIT, "Failed to show print dialog - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = true;
		}
	}

	return success;
}

bool gui::statement_page::email()
{
	bool success{false};
	if (this->documents.empty())
	{
		if (this->no_item_selected.show() == false)
		{
			syslog(LOG_CRIT, "Failed to show no_item_selected dialog - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
	}
	else
	{
		if (this->email_alert.show() == false)
		{
			syslog(LOG_CRIT, "Failed to show email dialog - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = true;
		}
	}

	return success;
}

bool gui::statement_page::save()
{
	bool success{false};
	(void) this->save_alert.show();

	return success;
}

void gui::statement_page::email_sent()
{
	if (this->email_future.get() == false)
	{
		syslog(LOG_CRIT, "Failed to send the email - "
				 "filename %s, line number %d", __FILE__, __LINE__);
		// No internet;
	}
	else
	{
		syslog(LOG_CRIT, "Email successfully sent - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
}
