/********************************************************
 * Contents: Admin System class definition
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _ADMIN_SYSTEM_H_
#define _ADMIN_SYSTEM_H_
#include <gtkmm.h>
#include <main_window.h>
#include <business_page.h>
#include <client_register_page.h>
#include <invoice_page.h>

namespace app {
        class admin_system {
                public:
                        admin_system() = default;
                        admin_system(const admin_system&) = delete;
                        admin_system(admin_system&&) = delete;
                        admin_system& operator= (const admin_system&) = delete;
                        admin_system& operator= (admin_system&&) = delete;
                        ~admin_system() = default;

                        void start(const Glib::RefPtr<Gtk::Application>&);

                private:
                        [[nodiscard]] bool verify_ui_file(const Glib::RefPtr<Gtk::Builder>&);

                private:
                        gui::main_window main_window;
                        gui::business_page business_page{};
                        gui::client_register_page client_register_page{};
                        gui::invoice_page invoice_page{};
        };
}
#endif
