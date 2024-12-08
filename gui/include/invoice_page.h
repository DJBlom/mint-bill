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
        struct column_entries : public Glib::Object {
                public:
                        unsigned int quantity{0};
                        std::string description{""};
                        double amount{0.0};

                        static Glib::RefPtr<column_entries> create()
                        {
                                return Glib::make_refptr_for_instance<column_entries>(new column_entries());
                        }

                protected:
                        column_entries() {}
        };

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
                        void connect_search(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_save_button(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_save_alert(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_wrong_info_alert(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_wrong_data_in_quantity_column_alert(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_wrong_data_in_amount_column_alert(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_grand_total_label(const Glib::RefPtr<Gtk::Builder>&);

                        void connect_description_view(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_description_add_button(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_description_delete_button(const Glib::RefPtr<Gtk::MultiSelection>&, const Glib::RefPtr<Gtk::Builder>&);
                        void connect_description_list_store(const Glib::RefPtr<Gtk::Builder>&);
                        void update_description_total(uint, uint, uint);

                        void connect_material_view(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_material_add_button(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_material_delete_button(const Glib::RefPtr<Gtk::MultiSelection>&, const Glib::RefPtr<Gtk::Builder>&);
                        void connect_material_list_store(const Glib::RefPtr<Gtk::Builder>&);
                        void update_material_total(uint, uint, uint);

                        void quantity_column(const std::shared_ptr<Gtk::ColumnView>&);
                        void description_column(const std::shared_ptr<Gtk::ColumnView>&);
                        void amount_column(const std::shared_ptr<Gtk::ColumnView>&);
                        void setup(const Glib::RefPtr<Gtk::ListItem>&);
                        void teardown(const Glib::RefPtr<Gtk::ListItem>& list_item);
                        void bind_quantity(const Glib::RefPtr<Gtk::ListItem>&);
                        void bind_description(const Glib::RefPtr<Gtk::ListItem>&);
                        void bind_amount(const Glib::RefPtr<Gtk::ListItem>&);

                        double compute_total(const Glib::RefPtr<Gio::ListStore<column_entries>>&);
                        double compute_grand_total();

                private:
                        std::unique_ptr<Gtk::MessageDialog> wrong_info_alert_dialog;
                        std::unique_ptr<Gtk::MessageDialog> save_alert_dialog;
                        std::unique_ptr<Gtk::MessageDialog> wrong_data_in_quantity_column;
                        std::unique_ptr<Gtk::MessageDialog> wrong_data_in_amount_column;
                        Glib::RefPtr<Gio::ListStore<column_entries>> description_store;
                        Glib::RefPtr<Gio::ListStore<column_entries>> material_store;
                        std::unique_ptr<Gtk::Label> description_total;
                        std::unique_ptr<Gtk::Label> material_total;
                        std::unique_ptr<Gtk::Label> grand_total;
        };
}
#endif
