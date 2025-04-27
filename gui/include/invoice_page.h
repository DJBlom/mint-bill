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
#include <sql.h>
#include <mutex>
#include <regex>
#include <vector>
#include <iomanip>
#include <sstream>
#include <column_data.h>
#include <invoice_data.h>
#include <client_invoice.h>
//#include <gui.h>
//#include <sql.h>
#include <pdf.h>
//#include <vector>
#include <thread>
//#include <client_invoice.h>
#include <pdf_invoice_data.h>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-image.h>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page-renderer.h>
//#include <known_invoices.h>

namespace gui {
        struct column_entries : public Glib::Object {
                public:
                        unsigned int quantity{0};
                        std::string description{""};
                        double amount{0.0};

                        static Glib::RefPtr<column_entries> create()
                        {
                                return Glib::make_refptr_for_instance<column_entries>(new column_entries());
                        }

                        static Glib::RefPtr<column_entries> create(const unsigned int _quantity,
                                                                   const std::string _description,
                                                                   const double _amount)
                        {
                                return Glib::make_refptr_for_instance<column_entries>(new column_entries(
                                                        _quantity, _description, _amount));
                        }

                protected:
                        column_entries() {}
                        explicit column_entries(const unsigned int _quantity,
                                                const std::string _description,
                                                const double _amount)
                                                : quantity{_quantity},
                                                  description{_description},
                                                  amount{_amount} {}
        };

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

        class invoice_page : public interface::gui {
                public:
                        //invoice_page() = default;
                        invoice_page();
                        invoice_page(const invoice_page&) = delete;
                        invoice_page(invoice_page&&) = delete;
                        invoice_page& operator= (const invoice_page&) = delete;
                        invoice_page& operator= (invoice_page&&) = delete;
                        ~invoice_page() override;

                        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;

                private: // GUI creations
                        void create_views(const Glib::RefPtr<Gtk::Builder>&);
                        void create_entries(const Glib::RefPtr<Gtk::Builder>&);
                        void create_dialogs(const Glib::RefPtr<Gtk::Builder>&);
                        void create_buttons(const Glib::RefPtr<Gtk::Builder>&);

                private: // Button events
                        void connect_search();
                        void connect_save_button();
                        void connect_material_add_button();
                        void connect_material_delete_button(const Glib::RefPtr<Gtk::MultiSelection>&);
                        void connect_description_add_button();
                        void connect_description_delete_button(const Glib::RefPtr<Gtk::MultiSelection>&);

// Integration starts
                private: // Email events
                        void connect_email_alert();
                        void connect_email_button();
                        void connect_no_internet_alert();
                        void send_email(const data::invoice&);
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
                        void populate(const std::string&);
                        void add(const data::invoice&);
                        void edit_selected_invoice(const data::invoice&);
                        void connect_invoice_view();
                        void invoices(const std::unique_ptr<Gtk::ListView>&);
                        void invoice_setup(const Glib::RefPtr<Gtk::ListItem>&);
                        void bind_invoices(const Glib::RefPtr<Gtk::ListItem>&);
                        void invoice_teardown(const Glib::RefPtr<Gtk::ListItem>&);
                        void populate_list_store(const std::vector<data::invoice>&);
                        void selected_invoice(uint, uint);

                private:
//                        storage::sql db{};
                        feature::pdf pdf{};
                        bool print_success{false};
                        bool email_success{false};
                        Glib::Dispatcher print_dispatcher{};
                        Glib::Dispatcher email_dispatcher{};
//                        data::invoice current_invoice{};
//                        feature::invoice client_invoice{};
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
// Integration ends








                private: // Dialog events
                        void connect_save_alert();
                        void connect_wrong_info_alert();
                        void connect_wrong_data_in_amount_column_alert();
                        void connect_wrong_data_in_quantity_column_alert();

                private: // View events
                        void connect_material_view();
                        void connect_description_view();
                        void connect_material_list_store();
                        void connect_description_list_store();
                        void edit_known_invoice(uint);

                private: // helper
                        void perform_search();
                        void on_search_changed();
                        void setup(const Glib::RefPtr<Gtk::ListItem>&);
                        void teardown(const Glib::RefPtr<Gtk::ListItem>&);
                        void bind_amount(const Glib::RefPtr<Gtk::ListItem>&);
                        void bind_quantity(const Glib::RefPtr<Gtk::ListItem>&);
                        void bind_description(const Glib::RefPtr<Gtk::ListItem>&);
                        void amount_column(const std::unique_ptr<Gtk::ColumnView>&);
                        void quantity_column(const std::unique_ptr<Gtk::ColumnView>&);
                        void description_column(const std::unique_ptr<Gtk::ColumnView>&);
                        void update_material_total(uint, uint, uint);
                        void update_description_total(uint, uint, uint);
                        [[nodiscard]] double compute_grand_total();
                        [[nodiscard]] data::invoice extract_invoice_data();
                        [[nodiscard]] double compute_total(const Glib::RefPtr<Gio::ListStore<column_entries>>&);
                        [[nodiscard]] std::vector<data::column> retrieve_column_data(const Glib::RefPtr<Gio::ListStore<column_entries>>&);

                private: // Member features
                        storage::sql db{};
//                        layout::known_invoices known_invoices{};
                        std::string grand_total{""};
                        std::string material_total{""};
                        std::string description_total{""};
                        data::invoice current_invoice{};
                        feature::invoice client_invoice{};

                private: // Member Entries
                        std::unique_ptr<Gtk::Entry> job_card{};
                        std::unique_ptr<Gtk::Entry> order_number{};
                        std::unique_ptr<Gtk::Entry> invoice_date{};
                        std::unique_ptr<Gtk::Entry> invoice_number{};
                        std::unique_ptr<Gtk::SearchEntry> search_entry{};

                private: // Member Views
                        std::unique_ptr<Gtk::ColumnView> material_view{};
                        std::unique_ptr<Gtk::ColumnView> description_view{};
                        std::shared_ptr<Gtk::Adjustment> material_adjustment{};
                        std::shared_ptr<Gtk::Adjustment> description_adjustment{};
                        std::shared_ptr<Gio::ListStore<column_entries>> material_store{};
                        std::shared_ptr<Gio::ListStore<column_entries>> description_store{};

                private: // Member buttons
                        std::unique_ptr<Gtk::Button> save_button{};
                        std::unique_ptr<Gtk::Button> material_add_button{};
                        std::unique_ptr<Gtk::Button> material_delete_button{};
                        std::unique_ptr<Gtk::Button> description_add_button{};
                        std::unique_ptr<Gtk::Button> description_delete_button{};

                private: // Member labels
                        std::unique_ptr<Gtk::Label> grand_total_label{};
                        std::unique_ptr<Gtk::Label> material_total_label{};
                        std::unique_ptr<Gtk::Label> description_total_label{};

                private: // Member Message dialogs
                        std::unique_ptr<Gtk::MessageDialog> save_alert_dialog{};
                        std::unique_ptr<Gtk::MessageDialog> wrong_info_alert_dialog{};
                        std::unique_ptr<Gtk::MessageDialog> wrong_data_in_amount_column{};
                        std::unique_ptr<Gtk::MessageDialog> wrong_data_in_quantity_column{};
        };
}
#endif
