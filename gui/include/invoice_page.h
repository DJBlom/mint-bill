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
        class column_entries : public Glib::Object {
                public:
                        Gtk::Entry* entry{nullptr};

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

                        void connect_description_view(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_description_add_button(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_description_delete_button(const Glib::RefPtr<Gtk::MultiSelection>&, const Glib::RefPtr<Gtk::Builder>&);

                        void connect_material_view(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_material_add_button(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_material_delete_button(const Glib::RefPtr<Gtk::MultiSelection>&, const Glib::RefPtr<Gtk::Builder>&);

                        void quantity_column(const std::unique_ptr<Gtk::ColumnView>&);
                        void description_column(const std::unique_ptr<Gtk::ColumnView>&);
                        void amount_column(const std::unique_ptr<Gtk::ColumnView>&);
                        void setup(const Glib::RefPtr<Gtk::ListItem>&);
                        void teardown(const Glib::RefPtr<Gtk::ListItem>& list_item);
                        void bind_quantity(const Glib::RefPtr<Gtk::ListItem>&);
                        void bind_description(const Glib::RefPtr<Gtk::ListItem>&);
                        void bind_amount(const Glib::RefPtr<Gtk::ListItem>&);


                private:
                        std::unique_ptr<Gtk::MessageDialog> wrong_info_alert_dialog;
                        std::unique_ptr<Gtk::MessageDialog> save_alert_dialog;
                        Glib::RefPtr<Gio::ListStore<column_entries>> description_store;
                        Glib::RefPtr<Gio::ListStore<column_entries>> material_store;
                        double description_total{0.0};
        };
}
#endif
