/********************************************************
 * Contents: Admin System class implementation
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 *******************************************************/
#include <iostream>
#include <admin_system.h>



namespace error {
        const std::string message_file{"Error: failed to load the ui file."};
        const std::string message_main_window{"Error: failed to create the main window."};
        const std::string message_business{"Error: failed to create the business page."};
        const std::string message_client{"Error: failed to create the client registration page."};
        const std::string message_invoice{"Error: failed to create the invoice page."};
}



void app::admin_system::start(const Glib::RefPtr<Gtk::Application>& app)
{
        Glib::RefPtr<Gtk::Builder> ui_builder;
        ui_builder = Gtk::Builder::create();
        if (this->verify_ui_file(ui_builder) == false)
        {
                std::cerr << error::message_file << std::endl;
                return;
        }
        else
        {
                if (this->main_window.create(ui_builder, app) == false)
                {
                        std::cerr << error::message_main_window << std::endl;
                        return;
                }

                if (this->business_page.create(ui_builder) == false)
                {
                        std::cerr << error::message_business << std::endl;
                        return;
                }

                if (this->client_register_page.create(ui_builder) == false)
                {
                        std::cerr << error::message_client << std::endl;
                        return;
                }

                if (this->invoice_page.create(ui_builder) == false)
                {
                        std::cerr << error::message_invoice << std::endl;
                        return;
                }
        }
}

bool app::admin_system::verify_ui_file(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool verified{false};
        try
        {
                verified = true;
                ui_builder->add_from_file("gui/admin-system.ui");
        }
        catch(const Glib::FileError& ex)
        {
                std::cerr << "FileError: " << ex.what() << std::endl;
                return verified;
        }
        catch(const Glib::MarkupError& ex)
        {
                std::cerr << "MarkupError: " << ex.what() << std::endl;
                return verified;
        }
        catch(const Gtk::BuilderError& ex)
        {
                std::cerr << "BuilderError: " << ex.what() << std::endl;
                return verified;
        }

        return verified;
}
