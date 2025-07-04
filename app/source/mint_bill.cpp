/********************************************************
 * Contents: Admin System class implementation
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 *******************************************************/
#include <syslog.h>
#include <mint_bill.h>
#include <main_window.h>
#include <invoice_page.h>
#include <business_page.h>
#include <statement_page.h>
#include <client_register_page.h>


void app::mint_bill::start(const Glib::RefPtr<Gtk::Application>& app)
{
        Glib::RefPtr<Gtk::Builder> ui_builder{Gtk::Builder::create()};
        syslog(LOG_INFO, "Starting...");
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

                if (this->invoice_page.create(ui_builder) == false)
                {
                        syslog(LOG_CRIT, "Failed to create the invoice page - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
                        return;
                }

                if (this->statement_page.create(ui_builder) == false)
                {
                        syslog(LOG_CRIT, "Failed to create the statement page - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
                        return;
                }



		(void) this->stack.create(ui_builder);
		(void) this->search_bar.create(ui_builder);
		(void) this->print_button.create(ui_builder);
		(void) this->print_button.enable();
		(void) this->email_button.create(ui_builder);
		(void) this->email_button.enable();
		(void) this->save_button.create(ui_builder);
		(void) this->save_button.enable();

		(void) this->stack.subscribe("search_bar", [this] (const std::string& _stack_page_name) {
			(void) this->search_bar.update(_stack_page_name);
			if (_stack_page_name == "invoice-page")
			{
				(void) this->print_button.enable();
				(void) this->email_button.enable();
				(void) this->save_button.enable();
			}
			else if (_stack_page_name == "statement-page")
			{
				(void) this->print_button.enable();
				(void) this->email_button.enable();
				(void) this->save_button.enable();
			}
			else if (_stack_page_name == "register-page")
			{
				(void) this->print_button.disable();
				(void) this->email_button.disable();
				(void) this->save_button.enable();
			}
			else if (_stack_page_name == "business-page")
			{
				(void) this->print_button.disable();
				(void) this->email_button.disable();
				(void) this->save_button.enable();
			}
			else
			{
				syslog(LOG_CRIT, "The stack page is not known: %s", _stack_page_name.c_str());
			}

			return true;
		});

		(void) this->search_bar.subscribe("search", [this] (const std::string& _keyword) {
			if (this->stack.current_page() == "invoice-page")
			{
				(void) this->invoice_page.search(_keyword);
			}
			else if (this->stack.current_page() == "statement-page")
			{
				(void) this->statement_page.search(_keyword);
			}
			else if (this->stack.current_page() == "register-page")
			{
				(void) this->client_register_page.search(_keyword);
			}
			else if (this->stack.current_page() == "business-page")
			{
				(void) this->business_page.search(_keyword);
			}
			else
			{
				syslog(LOG_CRIT, "Search function not implemented for: %s", this->stack.current_page().c_str());
			}

			return true;
		});

		(void) this->print_button.subscribe([this] () {
			if (this->stack.current_page() == "invoice-page")
			{
				(void) this->invoice_page.print();
			}
			else if (this->stack.current_page() == "statement-page")
			{
				(void) this->statement_page.print();
			}
			else
			{
				syslog(LOG_CRIT, "The print function not implemented for: %s", this->stack.current_page().c_str());
			}

			return true;
		});

		(void) this->email_button.subscribe([this] () {
			if (this->stack.current_page() == "invoice-page")
			{
				(void) this->invoice_page.email();
			}
			else if (this->stack.current_page() == "statement-page")
			{
				(void) this->statement_page.email();
			}
			else
			{
				syslog(LOG_CRIT, "The email function not implemented for: %s", this->stack.current_page().c_str());
			}

			return true;
		});

		(void) this->save_button.subscribe([this] () {
			if (this->stack.current_page() == "invoice-page")
			{
				(void) this->invoice_page.save();
			}
			else if (this->stack.current_page() == "statement-page")
			{
				(void) this->statement_page.save();
			}
			else if (this->stack.current_page() == "register-page")
			{
				(void) this->client_register_page.save();
			}
			else if (this->stack.current_page() == "business-page")
			{
				(void) this->business_page.save();
			}
			else
			{
				syslog(LOG_CRIT, "The save function not implemented for: %s", this->stack.current_page().c_str());
			}

			return true;
		});
        }
}

bool app::mint_bill::load_ui_file(const Glib::RefPtr<Gtk::Builder>& ui_builder)
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
