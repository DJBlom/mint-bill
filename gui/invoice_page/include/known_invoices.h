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
#include <pdf.h>
#include <vector>
#include <thread>
#include <invoice_data.h>
#include <client_invoice.h>
#include <pdf_invoice_data.h>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-image.h>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page-renderer.h>

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
                        virtual void add(const data::invoice&);

                private:
                        void create_ui(const Glib::RefPtr<Gtk::Builder>&);

                private: // Email
                        void send_email(const data::invoice&);
                        void connect_email_alert();
                        void connect_email_button();
                        void connect_no_internet_alert();
                        void email_sent();

                private: // Print
                        void setup_page();
                        void setup_print_operation();
                        void print_invoice(const data::invoice&);
                        void connect_printer();
                        void connect_print_alert();
                        void connect_print_button();
                        void connect_no_printer_alert();
                        void on_draw_page(const Glib::RefPtr<Gtk::PrintContext>&, int);
                        void on_printoperation_done(Gtk::PrintOperation::Result, const Glib::RefPtr<Gtk::PrintOperation>&);
                        void printed();

                private: // Invoice
                        void connect_invoice_view();
                        void connect_invoice_list_store();
                        void invoices(const std::unique_ptr<Gtk::ListView>&);
                        void invoice_setup(const Glib::RefPtr<Gtk::ListItem>&);
                        void bind_invoices(const Glib::RefPtr<Gtk::ListItem>&);
                        void invoice_teardown(const Glib::RefPtr<Gtk::ListItem>&);
                        void populate_list_store(const std::vector<data::invoice>&);
                        void selected_invoice(uint);

                private:
                        storage::sql db{};
                        feature::pdf pdf{};
                        bool print_success{false};
                        bool email_success{false};
                        Glib::Dispatcher print_dispatcher{};
                        Glib::Dispatcher email_dispatcher{};
                        data::invoice current_invoice{};
                        feature::invoice client_invoice{};
                        std::shared_ptr<poppler::document> document{};

                private:
                        std::unique_ptr<Gtk::Button> email_button{};
                        std::unique_ptr<Gtk::Button> print_button{};
                        std::unique_ptr<Gtk::ListView> invoice_view{};
                        Glib::RefPtr<Gtk::PageSetup> page_setup{};
                        Glib::RefPtr<Gtk::PrintOperation> print_operation{};
                        std::shared_ptr<Gtk::Adjustment> invoices_adjustment{};
                        std::unique_ptr<Gtk::MessageDialog> print_no_printer{};
                        std::unique_ptr<Gtk::MessageDialog> email_no_internet{};
                        std::unique_ptr<Gtk::MessageDialog> print_confirmation{};
                        std::unique_ptr<Gtk::MessageDialog> email_confirmation{};
                        std::shared_ptr<Gio::ListStore<invoice_entries>> invoice_store{};
        };
}
#endif
