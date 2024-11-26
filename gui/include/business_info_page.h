/********************************************************
 * Contents: Business Information Page class definition
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _BUSINESS_INFO_PAGE_H_
#define _BUSINESS_INFO_PAGE_H_
#include <gui.h>
#include <sql.h>
#include <business_info.h>

namespace gui {
        class business_info_page : public interface::gui {
                public:
                        business_info_page() = default;
                        business_info_page(const business_info_page&) = delete;
                        business_info_page(business_info_page&&) = delete;
                        business_info_page& operator= (const business_info_page&) = delete;
                        business_info_page& operator= (business_info_page&&) = delete;
                        ~business_info_page() override;

                        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;

                private:
                        [[nodiscard]] bool verify_ui_builder(const Glib::RefPtr<Gtk::Builder>&);
                        void update_business_info_with_db_data(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_wrong_info_alert(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_save_alert(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_save_button(const Glib::RefPtr<Gtk::Builder>&);
                        data::business extract_page_entries(const Glib::RefPtr<Gtk::Builder>&);

                private:
                        storage::sql sql{};
                        feature::business business_info{};
                        Gtk::MessageDialog* wrong_info_alert_dialog{};
                        Gtk::MessageDialog* save_alert_dialog{};
        };
}
#endif
