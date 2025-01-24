/********************************************************
 * Contents: Known Invoices class definition
 * Author: Dawid J. Blom
 * Date: January 24, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _KNOWN_INVOICES_H_
#define _KNOWN_INVOICES_H_
#include <gui.h>
#include <sql.h>
#include <vector>
#include <thread>
#include <client_invoice.h>

namespace layout {
        struct invoice_entries : public Glib::Object {
                public:
                        data::invoice invoice{};

                        static Glib::RefPtr<invoice_entries> create(const data::invoice& _invoice)
                        {
                                return Glib::make_refptr_for_instance<invoice_entries>(new invoice_entries(_invoice));
                        }

                protected:
                        explicit invoice_entries(const data::invoice& _invoice) : invoice{_invoice} {}
        };

        class known_invoices : public interface::gui {
                public:
                        known_invoices();
                        known_invoices(const known_invoices&) = delete;
                        known_invoices(known_invoices&&) = delete;
                        known_invoices& operator= (const known_invoices&) = delete;
                        known_invoices& operator= (known_invoices&&) = delete;
                        ~known_invoices() override;

                        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;
                        virtual void populate(const std::string&);
                        virtual void send_email(const data::invoice&);

                private:
                        void create_ui(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_email_alert();
                        void connect_email_button();
                        void connect_invoice_view();
                        void connect_no_internet_alert();
                        void connect_invoice_list_store();
                        void invoices(const std::unique_ptr<Gtk::ListView>&);
                        void invoice_setup(const Glib::RefPtr<Gtk::ListItem>&);
                        void bind_invoices(const Glib::RefPtr<Gtk::ListItem>&);
                        void invoice_teardown(const Glib::RefPtr<Gtk::ListItem>&);
                        void populate_list_store(const std::vector<data::invoice>&);
                        void selected_invoice(uint);
                        void email_sent();

                private:
                        storage::sql db{};
                        bool email_success{false};
                        Glib::Dispatcher dispatcher{};
                        data::invoice current_invoice{};
                        feature::invoice client_invoice{};

                private:
                        std::unique_ptr<Gtk::Button> email_button{};
                        std::unique_ptr<Gtk::ListView> invoice_view{};
                        std::shared_ptr<Gtk::Adjustment> invoices_adjustment{};
                        std::unique_ptr<Gtk::MessageDialog> email_no_internet{};
                        std::unique_ptr<Gtk::MessageDialog> email_confirmation{};
                        std::shared_ptr<Gio::ListStore<invoice_entries>> invoice_store{};
        };
}
#endif
