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
#include <config.h>
#include <invoice_model.h>


gui::statement_page::statement_page()
{
        this->email_dispatcher.connect(sigc::mem_fun(*this, &statement_page::email_sent));
}

gui::statement_page::~statement_page() {}

bool gui::statement_page::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder,
				 const std::shared_ptr<Gtk::Window>& _main_window)
{
        if (_ui_builder == nullptr || _main_window == nullptr)
	{
		syslog(LOG_CRIT, "STATEMENT_PAGE: The _ui_builder or the _main_window is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
		return false;
	}
	else
        {
		this->total_label = std::unique_ptr<Gtk::Label>{
			_ui_builder->get_widget<Gtk::Label>("statement-total-label")};

		if (this->email_setup(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to setup email - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			return false;
		}

		if (this->print_setup(_ui_builder, _main_window) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to setup print - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			return false;
		}

		if (this->save_setup(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to setup save - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			return false;
		}

		if (this->no_item_selected_setup(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to setup no_item_selected_setup - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			return false;
		}

		if (this->no_internet_setup(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to setup no_internet_setup - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			return false;
		}

		if (this->invoice_pdf_view_setup(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to setup invoice_pdf_view_setup - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			return false;
		}

		if (this->statement_pdf_view_setup(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to setup statement_pdf_view_setup - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			return false;
		}

		if (this->statement_column_view_setup(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to setup statement_column_view_setup - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			return false;
		}
        }

        return true;
}

bool gui::statement_page::set_database_password(const std::string& _database_password)
{
	bool success{false};
	if (_database_password.empty() == true)
	{
                syslog(LOG_CRIT, "STATEMENT_PAGE: database password is empty - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->database_password = _database_password;
	}

	return success;
}

bool gui::statement_page::search(const std::string& _keyword)
{
        bool searched{true};
	this->clear();
	if (_keyword.empty() == true)
	{
		searched = false;
		syslog(LOG_CRIT, "STATEMENT_PAGE: The _keyword is empty, clearing all entries - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (this->populate(_keyword) == false)
		{
			searched = false;
		}
	}

        return searched;
}

bool gui::statement_page::print()
{
	bool success{false};
	if (this->documents.empty() == true)
	{
		if (this->no_item_selected.show() == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to show no_item_selected dialog - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
	}
	else
	{
		if (this->print_alert.show() == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to show print dialog - "
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
	if (this->documents.empty() == true)
	{
		if (this->no_item_selected.show() == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to show no_item_selected dialog - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
	}
	else
	{
		if (this->email_alert.show() == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to show email dialog - "
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
	this->invoice_data = this->statement_view.extract();
	if (invoice_data.empty() == true)
	{
		if (this->no_item_selected.show() == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to show no_item_selected dialog - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
	}
	else
	{
		success = true;
		if (this->save_alert.show() == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to show save dialog - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
	}

	return success;
}

bool gui::statement_page::email_setup(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (_ui_builder == nullptr)
	{
		syslog(LOG_CRIT, "STATEMENT_PAGE: The _ui_builder or the _main_window is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (this->email_alert.create(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to create email dialog - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = this->email_alert.connect([this] (const int& response) {
				switch (response)
				{
					case GTK_RESPONSE_YES:
						if (this->email_alert.hide() == false)
						{
							syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to hide email dialog - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
						else
						{
							this->email_future = std::move(std::async(std::launch::async, [this] () {
								model::statement statement_model{app::config::path_to_database_file,
												 this->database_password};
								data::email data{statement_model.prepare_for_email(this->documents)};
								feature::email email;
								bool result{email.send(data)};
								this->email_dispatcher.emit();
								return result;
							}));
						}
						break;
					case GTK_RESPONSE_NO:
						if (this->email_alert.hide() == false)
						{
							syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to hide email dialog - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
						break;
					default:
						if (this->email_alert.hide() == false)
						{
							syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to hide email dialog - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
						break;
				}
			});
		}
	}

	return success;
}

bool gui::statement_page::print_setup(const Glib::RefPtr<Gtk::Builder>& _ui_builder,
				      const std::shared_ptr<Gtk::Window>& _main_window)
{
	bool success{false};
	if (_ui_builder == nullptr || _main_window == nullptr)
	{
		syslog(LOG_CRIT, "STATEMENT_PAGE: The _ui_builder or the _main_window is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (this->print_alert.create(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to create print dialog - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = this->print_alert.connect([this, _main_window] (const int& response) {
				switch (response)
				{
					case GTK_RESPONSE_YES:
						if (this->print_alert.hide() == false)
						{
							syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to hide print dialog - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
						else
						{
							model::statement statement_model{app::config::path_to_database_file,
											 this->database_password};
							std::vector<std::string> data{statement_model.prepare_for_print(this->documents)};
							gui::part::printer printer{"statement"};
							if (printer.print(data, _main_window) == false)
							{
								syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to print documents - "
										 "filename %s, line number %d", __FILE__, __LINE__);
							}
						}
						break;
					case GTK_RESPONSE_NO:
						if (this->print_alert.hide() == false)
						{
							syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to hide print dialog - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
						break;
					default:
						if (this->print_alert.hide() == false)
						{
							syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to hide print dialog - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
						break;
				}
			});
		}
	}

	return success;
}

bool gui::statement_page::save_setup(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (_ui_builder == nullptr)
	{
		syslog(LOG_CRIT, "STATEMENT_PAGE: The _ui_builder is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (this->save_alert.create(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to create save dialog - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = this->save_alert.connect([this] (const int& response) {
				switch (response)
				{
					case GTK_RESPONSE_YES:
						if (this->save_alert.hide() == false)
						{
							syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to hide save dialog - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
						else
						{
							model::statement statement_model{app::config::path_to_database_file, this->database_password};
							data::statement statement_data{this->selected_pdf_statement.get_statement()};
							if (statement_model.save(statement_data) == false)
							{
								syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to save statement - "
										 "filename %s, line number %d", __FILE__, __LINE__);
							}
							else
							{
								model::invoice invoice_model{app::config::path_to_database_file,
											     this->database_password};
								for (const std::any& data : this->invoice_data)
								{
									data::invoice invoice{std::any_cast<data::invoice> (data)};
									if (invoice_model.save(invoice) == false)
									{
										syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to save associated invoice - "
												 "filename %s, line number %d", __FILE__, __LINE__);
									}
								}

								this->clear();
								if (this->populate(statement_data.get_name()) == false)
								{
									syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to re-populate - "
											 "filename %s, line number %d", __FILE__, __LINE__);
								}
							}
						}
						break;
					case GTK_RESPONSE_NO:
						if (this->save_alert.hide() == false)
						{
							syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to hide save dialog - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
						break;
					default:
						if (this->save_alert.hide() == false)
						{
							syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to hide save dialog - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
						break;
				}
			});
		}
	}

	return success;
}

bool gui::statement_page::populate(const std::string& _business_name)
{
	std::vector<std::any> pdf_statements{};
	model::statement statement_model{app::config::path_to_database_file, this->database_password};
	for (const std::any& data : statement_model.load(_business_name))
	{
		data::pdf_statement pdf_statement{std::any_cast<data::pdf_statement>(data)};
		pdf_statements.emplace_back(pdf_statement);
	}

	if (this->statement_pdf_view.populate(pdf_statements) == false)
	{
		return false;
	}

	return true;
}

bool gui::statement_page::no_item_selected_setup(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (_ui_builder == nullptr)
	{
		syslog(LOG_CRIT, "STATEMENT_PAGE: The _ui_builder is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (this->no_item_selected.create(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to create no_item_selected dialog - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = this->no_item_selected.connect([this] (const int& response) {
				switch (response)
				{
					case GTK_RESPONSE_CLOSE:
						if (this->no_item_selected.hide() == false)
						{
							syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to hide no_item_selected dialog - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
						break;
					default:
						if (this->no_item_selected.hide() == false)
						{
							syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to hide no_item_selected dialog - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
						break;
				}
			});
		}
	}

	return success;
}

bool gui::statement_page::no_internet_setup(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (_ui_builder == nullptr)
	{
		syslog(LOG_CRIT, "STATEMENT_PAGE: The _ui_builder is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (this->no_internet_alert.create(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to create no_item_selected dialog - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = this->no_internet_alert.connect([this] (const int& response) {
				switch (response)
				{
					case GTK_RESPONSE_CLOSE:
						if (this->no_internet_alert.hide() == false)
						{
							syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to hide no_internet_alert dialog - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
						break;
					default:
						if (this->no_internet_alert.hide() == false)
						{
							syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to hide no_internet_alert dialog - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
						break;
				}
			});
		}
	}

	return success;
}


bool gui::statement_page::statement_column_view_setup(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (_ui_builder == nullptr)
	{
		syslog(LOG_CRIT, "STATEMENT_PAGE: The _ui_builder is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (this->statement_view.create(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to create the statement_view - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			if (this->invoice_number.create("Invoice Number") == false)
			{
				syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to create the column Invoice Number - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				if (this->statement_view.add_column(this->invoice_number) == false)
				{
					syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to add the column invoice_number - "
							 "filename %s, line number %d", __FILE__, __LINE__);
				}
			}

			if (this->date.create("Date") == false)
			{
				syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to create the column Date - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				if (this->statement_view.add_column(this->date) == false)
				{
					syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to add the column date - "
							 "filename %s, line number %d", __FILE__, __LINE__);
				}
			}

			if (this->order_number.create("Order Number") == false)
			{
				syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to create the column Order Number - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				if (this->statement_view.add_column(this->order_number) == false)
				{
					syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to add the column order_number - "
							 "filename %s, line number %d", __FILE__, __LINE__);
				}
			}

			if (this->paid_status.create("Paid Status") == false)
			{
				syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to create the column Paid Status - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				if (this->statement_view.add_column(this->paid_status) == false)
				{
					syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to add the column paid_satus - "
							 "filename %s, line number %d", __FILE__, __LINE__);
				}
			}

			if (this->price.create("Price") == false)
			{
				syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to create the column Price - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				if (this->statement_view.add_column(this->price) == false)
				{
					syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to add the column price - "
							 "filename %s, line number %d", __FILE__, __LINE__);
				}
			}
			success = true;
		}
	}

	return success;
}

bool gui::statement_page::invoice_pdf_view_setup(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (this->invoice_pdf_view.create(_ui_builder) == false)
	{
		syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to create the invoice_pdf_view - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
	}

	return success;
}

bool gui::statement_page::statement_pdf_view_setup(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (_ui_builder == nullptr)
	{
		syslog(LOG_CRIT, "STATEMENT_PAGE: The _ui_builder is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (this->statement_pdf_view.create(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to create the statement_pdf_view - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			if (on_double_click() == false)
			{
				syslog(LOG_CRIT, "STATEMENT_PAGE: The pdf_statement_data is not valid - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}

			if (on_single_click() == false)
			{
				syslog(LOG_CRIT, "STATEMENT_PAGE: The pdf_statements is not valid - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
			success = true;
		}
	}

	return success;
}

bool gui::statement_page::on_double_click()
{
	return this->statement_pdf_view.double_click([this] (const std::any& _data) {
		bool success{true};
		data::pdf_statement pdf_statement_data{std::any_cast<data::pdf_statement> (_data)};
		if (pdf_statement_data.is_valid() == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: The pdf_statement data is not valid - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			success = false;
		}
		else
		{
			std::vector<std::any> invoices{};
			std::vector<std::any> pdf_invoices{};
			this->selected_pdf_statement = pdf_statement_data;
			this->total_label->set_text("Total: " + this->selected_pdf_statement.get_total());
			for (const data::pdf_invoice& pdf_invoice : pdf_statement_data.get_pdf_invoices())
			{
				if (pdf_invoice.is_valid() == false)
				{
					syslog(LOG_CRIT, "STATEMENT_PAGE: The pdf_invoice data is not valid - "
							 "filename %s, line number %d", __FILE__, __LINE__);
				}
				else
				{
					invoices.emplace_back(pdf_invoice.get_invoice());
					pdf_invoices.emplace_back(pdf_invoice);
				}
			}

			if (this->statement_view.populate(invoices) == false)
			{
				syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to populate the statement_view - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}

			if (this->invoice_pdf_view.populate(pdf_invoices) == false)
			{
				syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to populate the invoice_pdf_view- "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}

		return success;
	});
}

bool gui::statement_page::on_single_click()
{
	return this->statement_pdf_view.single_click([this] (const std::vector<std::any>& _pdf_statements) {
		bool success{true};
		if (_pdf_statements.empty() == true)
		{
			success = false;
		}
		else
		{
			this->documents.clear();
			this->documents = _pdf_statements;
		}

		return success;
	});
}

void gui::statement_page::email_sent()
{
	if (this->email_future.get() == false)
	{
		syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to send the email - "
				 "filename %s, line number %d", __FILE__, __LINE__);
		if (this->no_internet_alert.show() == false)
		{
			syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to show the no_internet_alert - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
	}
	else
	{
		syslog(LOG_CRIT, "STATEMENT_PAGE: Email successfully sent - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
}

void gui::statement_page::clear()
{
	this->documents.clear();
	this->invoice_data.clear();
	this->total_label->set_text("Total: ");

	if (this->statement_view.clear() == false)
	{
		syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to clear statement_view - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}

	if (this->invoice_pdf_view.clear() == false)
	{
		syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to clear invoice_pdf_view - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}

	if (this->statement_pdf_view.clear() == false)
	{
		syslog(LOG_CRIT, "STATEMENT_PAGE: Failed to clear statement_pdf_view - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
}
