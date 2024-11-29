/********************************************************
 * Contents: Invoice Page class definition
 * Author: Dawid J. Blom
 * Date: November 28, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _INVOICE_PAGE_H_
#define _INVOICE_PAGE_H_
#include <gui.h>

namespace gui {
        class invoice_page : public interface::gui {
                public:
                        invoice_page() = default;
                        invoice_page(const invoice_page&) = delete;
                        invoice_page(invoice_page&&) = delete;
                        invoice_page& operator= (const invoice_page&) = delete;
                        invoice_page& operator= (invoice_page&&) = delete;
                        ~invoice_page() override;

                        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;

                private:
                        [[nodiscard]] bool verify_ui_builder(const Glib::RefPtr<Gtk::Builder>&);
//                        void display_on_ui(const data::client&, const Glib::RefPtr<Gtk::Builder>&);
//                        void connect_search(const Glib::RefPtr<Gtk::Builder>&);
//                        void connect_save_button(const Glib::RefPtr<Gtk::Builder>&);
//                        void connect_save_alert(const Glib::RefPtr<Gtk::Builder>&);
//                        void connect_wrong_info_alert(const Glib::RefPtr<Gtk::Builder>&);

        };
}
#endif
