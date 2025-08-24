/********************************************************************************
 * Contents: The main function
 * Author: Dawid Blom
 * Date: October 26, 2024
 *
 * Note:
 *******************************************************************************/
#include <gui.h>
#include <stack.h>
#include <syslog.h>
#include <gui_parts.h>
#include <main_window.h>
#include <invoice_page.h>
#include <business_page.h>
#include <statement_page.h>
#include <client_register_page.h>

class mint_bill {
public:
	mint_bill() = default;
	mint_bill(const mint_bill&) = delete;
	mint_bill(mint_bill&&) = delete;
	mint_bill& operator= (const mint_bill&) = delete;
	mint_bill& operator= (mint_bill&&) = delete;
	~mint_bill() = default;

	[[nodiscard]] bool create();
	[[nodiscard]] int launch(int argc, char** argv);

protected:
	void activate();
	[[nodiscard]] bool load_ui_file(const Glib::RefPtr<Gtk::Builder>&);
	[[nodiscard]] bool stack_setup(const Glib::RefPtr<Gtk::Builder>&);
	[[nodiscard]] bool search_bar_setup(const Glib::RefPtr<Gtk::Builder>&);
	[[nodiscard]] bool print_button_setup(const Glib::RefPtr<Gtk::Builder>&);
	[[nodiscard]] bool email_button_setup(const Glib::RefPtr<Gtk::Builder>&);
	[[nodiscard]] bool save_button_setup(const Glib::RefPtr<Gtk::Builder>&);

private:
	std::shared_ptr<Gtk::Application> app;
	gui::main_window main_window{};
	gui::business_page business_page{};
	gui::client_register_page client_register_page{};
	gui::invoice_page invoice_page{};
	gui::statement_page statement_page{};
	gui::part::stack stack{"business-stack"};
	gui::part::search_bar search_bar{"business-name-search"};
	gui::part::sub_button print_button{"print-button"};
	gui::part::sub_button email_button{"email-button"};
	gui::part::sub_button save_button{"save-button"};
};


int main(int argc, char** argv)
{
	mint_bill mint_bill;
	if (mint_bill.create() == false)
	{
                syslog(LOG_CRIT, "Failed to create mint_bill application - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}

	return mint_bill.launch(argc, argv);
}

bool mint_bill::create()
{
        this->app = Gtk::Application::create("org.engineering.mint-bill");
	if (this->app == nullptr)
	{
                syslog(LOG_CRIT, "The mint_bill application is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		return false;
	}
	else
	{
		this->app->signal_activate().connect(sigc::mem_fun(*this, &mint_bill::activate));
	}

	return true;
}

int mint_bill::launch(int argc, char** argv)
{
	return this->app->run(argc, argv);
}

void mint_bill::activate()
{
        Glib::RefPtr<Gtk::Builder> ui_builder{Gtk::Builder::create()};
        if (this->load_ui_file(ui_builder) == false)
        {
                syslog(LOG_CRIT, "Failed to load the UI file - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		return;
        }
        else
        {
                Glib::RefPtr<Gtk::Settings> system_settings = Gtk::Settings::get_default();
                if (!system_settings)
                {
                        syslog(LOG_CRIT, "Failed to get the default system settings - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
			return;
                }
                else
                {
			system_settings->property_gtk_cursor_blink() = false;
                        system_settings->property_gtk_application_prefer_dark_theme() = true;
                }

                if (this->main_window.create(ui_builder, app) == false)
                {
                        syslog(LOG_CRIT, "Failed to create the main window - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
			return;
                }

                if (this->business_page.create(ui_builder) == false)
                {
                        syslog(LOG_CRIT, "Failed to create the business page - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
			return;
                }

                if (this->client_register_page.create(ui_builder) == false)
                {
                        syslog(LOG_CRIT, "Failed to create the client registration page - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
			return;
                }

                if (this->invoice_page.create(ui_builder, this->main_window.retrieve()) == false)
                {
                        syslog(LOG_CRIT, "Failed to create the invoice page - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
			return;
                }

                if (this->statement_page.create(ui_builder, this->main_window.retrieve()) == false)
                {
                        syslog(LOG_CRIT, "Failed to create the statement page - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
			return;
                }

		if (this->print_button_setup(ui_builder) == false)
		{
                        syslog(LOG_CRIT, "Failed to setup the print button - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
			return;
		}

		if (this->email_button_setup(ui_builder) == false)
		{
                        syslog(LOG_CRIT, "Failed to setup the email button - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
			return;
		}

		if (this->save_button_setup(ui_builder) == false)
		{
                        syslog(LOG_CRIT, "Failed to setup the save button - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
			return;
		}

		if (this->search_bar_setup(ui_builder) == false)
		{
                        syslog(LOG_CRIT, "Failed to setup the search bar - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
			return;
		}

		if (this->stack_setup(ui_builder) == false)
		{
                        syslog(LOG_CRIT, "Failed to setup the stack - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
			return;
		}
        }
}

bool mint_bill::load_ui_file(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool verified{false};
        try
        {
                verified = true;
                ui_builder->add_from_file("gui/mint-bill.ui");
        }
        catch(const Glib::FileError& ex)
        {
                std::string error_string{ex.what()};
                syslog(LOG_CRIT, error_string.c_str());
                return verified;
        }
        catch(const Glib::MarkupError& ex)
        {
                std::string error_string{ex.what()};
                syslog(LOG_CRIT, error_string.c_str());
                return verified;
        }
        catch(const Gtk::BuilderError& ex)
        {
                std::string error_string{ex.what()};
                syslog(LOG_CRIT, error_string.c_str());
                return verified;
        }

        return verified;
}

bool mint_bill::stack_setup(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (_ui_builder == nullptr)
	{
		syslog(LOG_CRIT, "The _ui_builder is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (this->stack.create(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "Failed to create the stack - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = this->stack.subscribe("search_bar", [this] (const std::string& _stack_page_name) {
				if (this->search_bar.update(_stack_page_name) == false)
				{
					syslog(LOG_CRIT, "Failed to update the search_bar - "
							 "filename %s, line number %d", __FILE__, __LINE__);
					return false;
				}
				else
				{
					if (_stack_page_name == "invoice-page" || _stack_page_name == "statment-page")
					{
						// Verify this behaviour with client
						if (invoice_page.clear() == false)
						{
							syslog(LOG_CRIT, "Failed to clear the invoice_page - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}

						if (this->print_button.enable() == false)
						{
							syslog(LOG_CRIT, "Failed to enable the print_button - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}

						if (this->email_button.enable() == false)
						{
							syslog(LOG_CRIT, "Failed to enable the email_button - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}

						if (this->save_button.enable() == false)
						{
							syslog(LOG_CRIT, "Failed to enable the save_button - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
					}
					else if (_stack_page_name == "register-page" || _stack_page_name == "business-page")
					{
						if (this->print_button.disable() == false)
						{
							syslog(LOG_CRIT, "Failed to disable the print_button - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}

						if (this->email_button.disable() == false)
						{
							syslog(LOG_CRIT, "Failed to disable the email_button - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}

						if (this->save_button.enable() == false)
						{
							syslog(LOG_CRIT, "Failed to enable the save_button - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
					}
					else
					{
						syslog(LOG_CRIT, "The stack page is not known: %s", _stack_page_name.c_str());

						return false;
					}
				}

				return true;
			});
		}
	}

	return success;
}

bool mint_bill::search_bar_setup(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (_ui_builder == nullptr)
	{
		syslog(LOG_CRIT, "The _ui_builder is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (this->search_bar.create(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "Failed to create the search_bar - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = this->search_bar.subscribe("search", [this] (const std::string& _keyword) {
				if (this->stack.current_page() == "invoice-page")
				{
					if (this->invoice_page.search(_keyword) == false)
					{
						syslog(LOG_CRIT, "Failed to perform invoice_page search - "
								 "filename %s, line number %d", __FILE__, __LINE__);
					}
				}
				else if (this->stack.current_page() == "statement-page")
				{
					if (this->statement_page.search(_keyword) == false)
					{
						syslog(LOG_CRIT, "Failed to perform statement_page search - "
								 "filename %s, line number %d", __FILE__, __LINE__);
					}
				}
				else if (this->stack.current_page() == "register-page")
				{
					if (this->client_register_page.search(_keyword) == false)
					{
						syslog(LOG_CRIT, "Failed to perform register_page search - "
								 "filename %s, line number %d", __FILE__, __LINE__);
					}
				}
				else if (this->stack.current_page() == "business-page")
				{
					if (this->business_page.search(_keyword) == false)
					{
						syslog(LOG_CRIT, "Failed to perform business_page search - "
								 "filename %s, line number %d", __FILE__, __LINE__);
					}
				}
				else
				{
					syslog(LOG_CRIT, "Search function not implemented for: %s", this->stack.current_page().c_str());

					return false;
				}

				return true;
			});
		}
	}

	return success;
}

bool mint_bill::print_button_setup(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (_ui_builder == nullptr)
	{
		syslog(LOG_CRIT, "The _ui_builder is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (this->print_button.create(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "Failed to create the print_button - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			if (this->print_button.enable() == false)
			{
				syslog(LOG_CRIT, "Failed to enable the print_button - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				success = this->print_button.subscribe([this] () {
					if (this->stack.current_page() == "invoice-page")
					{
						if (this->invoice_page.print() == false)
						{
							syslog(LOG_CRIT, "Failed to perform the invoice_page print - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
					}
					else if (this->stack.current_page() == "statement-page")
					{
						if (this->statement_page.print() == false)
						{
							syslog(LOG_CRIT, "Failed to perform the statement_page print - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
					}
					else
					{
						syslog(LOG_CRIT, "The print function not implemented for: %s", this->stack.current_page().c_str());

						return false;
					}

					return true;
				});
			}
		}
	}

	return success;
}

bool mint_bill::email_button_setup(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (_ui_builder == nullptr)
	{
		syslog(LOG_CRIT, "The _ui_builder is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (this->email_button.create(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "Failed to create the email_button - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			if (this->email_button.enable() == false)
			{
				syslog(LOG_CRIT, "Failed to enable the email_button - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				success = this->email_button.subscribe([this] () {
					if (this->stack.current_page() == "invoice-page")
					{
						if (this->invoice_page.email() == false)
						{
							syslog(LOG_CRIT, "Failed to perform the invoice_page email - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
					}
					else if (this->stack.current_page() == "statement-page")
					{
						if (this->statement_page.email() == false)
						{
							syslog(LOG_CRIT, "Failed to perform the statement_page email - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
					}
					else
					{
						syslog(LOG_CRIT, "The print function not implemented for: %s", this->stack.current_page().c_str());

						return false;
					}

					return true;
				});
			}
		}
	}

	return success;
}

bool mint_bill::save_button_setup(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (_ui_builder == nullptr)
	{
		syslog(LOG_CRIT, "The _ui_builder is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (this->save_button.create(_ui_builder) == false)
		{
			syslog(LOG_CRIT, "Failed to create the save_button - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			if (this->save_button.enable() == false)
			{
				syslog(LOG_CRIT, "Failed to enable the save_button - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				success = this->save_button.subscribe([this] () {
					if (this->stack.current_page() == "invoice-page")
					{
						if (this->invoice_page.save() == false)
						{
							syslog(LOG_CRIT, "Failed to perform the invoice_page save - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
					}
					else if (this->stack.current_page() == "statement-page")
					{
						if (this->statement_page.save() == false)
						{
							syslog(LOG_CRIT, "Failed to perform the statement_page save - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
					}
					else if (this->stack.current_page() == "register-page")
					{
						if (this->client_register_page.save() == false)
						{
							syslog(LOG_CRIT, "Failed to perform the client_register_page save - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
					}
					else if (this->stack.current_page() == "business-page")
					{
						if (this->business_page.save() == false)
						{
							syslog(LOG_CRIT, "Failed to perform the business_page save - "
									 "filename %s, line number %d", __FILE__, __LINE__);
						}
					}
					else
					{
						syslog(LOG_CRIT, "The print function not implemented for: %s", this->stack.current_page().c_str());

						return false;
					}

					return true;
				});
			}
		}
	}

	return success;
}
