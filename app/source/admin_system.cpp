/********************************************************
 * Contents: Admin System class implementation
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 *******************************************************/
#include <syslog.h>
#include <main_window.h>
#include <admin_system.h>
#include <invoice_page.h>
#include <business_page.h>
#include <statement_page.h>
#include <client_register_page.h>


void app::admin_system::start(const Glib::RefPtr<Gtk::Application>& app)
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
		(void) this->sub_save_button.create(ui_builder);
		(void) this->sub_save_button.subscribe("business-page", [this] (const std::string& _stack_page_name) {
			syslog(LOG_CRIT, "Saving data from: %s", _stack_page_name.c_str());
			return true;
		});

		(void) this->stack.subscribe("search_bar", [this] (const std::string& _stack_page_name) {
			syslog(LOG_CRIT, "Switching stack page to: %s", _stack_page_name.c_str());
			(void) this->search_bar.update(_stack_page_name);
			(void) this->sub_save_button.enable();
			return true;
		});

		(void) this->search_bar.subscribe("search", [this] (const std::string& _keyword) {
			syslog(LOG_CRIT, "Current stack page name: %s", this->stack.current_page().c_str());
			if (this->stack.current_page() == "invoice-page")
			{
				syslog(LOG_CRIT, "Search function not implemented for: %s", this->stack.current_page().c_str());
				(void) this->invoice_page.search(_keyword);
			}
			else if (this->stack.current_page() == "statement-page")
			{
				syslog(LOG_CRIT, "Search function not implemented for: %s", this->stack.current_page().c_str());
				(void) this->statement_page.search(_keyword);
			}
			else if (this->stack.current_page() == "register-page")
			{
				syslog(LOG_CRIT, "Search function not implemented for: %s", this->stack.current_page().c_str());
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




        }
}

bool app::admin_system::load_ui_file(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool verified{false};
        try
        {
                verified = true;
                ui_builder->add_from_file("gui/admin-system.ui");
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
