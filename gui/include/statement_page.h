/********************************************************
 * Contents: Statement Page class definition
 * Author: Dawid J. Blom
 * Date: February 19, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _STATEMENT_PAGE_H_
#define _STATEMENT_PAGE_H_
#include <gui.h>
#include <syslog.h>
#include <gui_parts.h>


namespace gui {

class statement_page : public interface::gui {
public:
        statement_page() = default;
        statement_page(const statement_page&) = delete;
        statement_page(statement_page&&) = delete;
        statement_page& operator= (const statement_page&) = delete;
        statement_page& operator= (statement_page&&) = delete;
        ~statement_page() override;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;

private:
        void create_ui(const Glib::RefPtr<Gtk::Builder>&);

private:
        part::button save_button{"statement-save-button"};
        part::dialog save_dialog{"statement-save-button-alert"};
        part::button print_button{"statement-print-button"};
        part::dialog print_dialog{"statement-print-alert"};
        part::button email_button{"statement-email-button"};
        part::dialog email_dialog{"statement-email-alert"};

        //                private: // Email
        //                        void send_email(const data::invoice&);
        //                        void connect_email_alert();
        //                        void connect_email_button();
        //                        void connect_no_internet_alert();
        //                        void email_sent();
        //
        //                private: // Print
        //                        void setup_page();
        //                        void setup_print_operation();
        //                        void print_invoice(const data::invoice&);
        //                        void connect_printer();
        //                        void connect_print_alert();
        //                        void connect_print_button();
        //                        void connect_no_printer_alert();
        //                        void on_draw_page(const Glib::RefPtr<Gtk::PrintContext>&, int);
        //                        void on_printoperation_done(Gtk::PrintOperation::Result, const Glib::RefPtr<Gtk::PrintOperation>&);
        //                        void printed();
        //
        //                private: // Invoice
        //                        void connect_invoice_view();
        //                        void connect_invoice_list_store();
        //                        void invoices(const std::unique_ptr<Gtk::ListView>&);
        //                        void invoice_setup(const Glib::RefPtr<Gtk::ListItem>&);
        //                        void bind_invoices(const Glib::RefPtr<Gtk::ListItem>&);
        //                        void invoice_teardown(const Glib::RefPtr<Gtk::ListItem>&);
        //                        void populate_list_store(const std::vector<data::invoice>&);
        //                        void selected_invoice(uint);
        //
        //                private:
        //                        storage::sql db{};
        //                        feature::pdf pdf{};
        //                        bool print_success{false};
        //                        bool email_success{false};
        //                        Glib::Dispatcher print_dispatcher{};
        //                        Glib::Dispatcher email_dispatcher{};
        //                        data::invoice current_invoice{};
        //                        feature::invoice client_invoice{};
        //                        std::shared_ptr<poppler::document> document{};
        //
        //                private:
        //                        std::unique_ptr<Gtk::Button> email_button{};
        //                        std::unique_ptr<Gtk::Button> print_button{};
        //                        std::unique_ptr<Gtk::ListView> statement_view{};
        //                        Glib::RefPtr<Gtk::PageSetup> page_setup{};
        //                        Glib::RefPtr<Gtk::PrintOperation> print_operation{};
        //                        std::shared_ptr<Gtk::Adjustment> statment_adjustment{};
        //                        std::unique_ptr<Gtk::MessageDialog> print_no_printer{};
        //                        std::unique_ptr<Gtk::MessageDialog> email_no_internet{};
        //                        std::unique_ptr<Gtk::MessageDialog> print_confirmation{};
        //                        std::unique_ptr<Gtk::MessageDialog> email_confirmation{};
        //                        std::shared_ptr<Gio::ListStore<statement_entries>> statement_store{};
};
}


#endif
