/********************************************************
 * Contents: Client Register Page class definition
 * Author: Dawid J. Blom
 * Date: November 26, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _CLIENT_REGISTER_PAGE_H_
#define _CLIENT_REGISTER_PAGE_H_
#include <gui.h>

namespace gui {
        class client_register_page : public interface::gui {
                public:
                        client_register_page() = default;
                        client_register_page(const client_register_page&) = delete;
                        client_register_page(client_register_page&&) = delete;
                        client_register_page& operator= (const client_register_page&) = delete;
                        client_register_page& operator= (client_register_page&&) = delete;
                        ~client_register_page() override;

                        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;

                private:
                        [[nodiscard]] bool verify_ui_builder(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_wrong_info_alert(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_save_alert(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_save_button(const Glib::RefPtr<Gtk::Builder>&);

                private:
                        Gtk::MessageDialog* wrong_info_alert_dialog{};
                        Gtk::MessageDialog* save_alert_dialog{};
        };
}
#endif
