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
#include <business.h>

namespace gui {
        class business_info_page : public interface::gui {
                public:
                        explicit business_info_page(const std::string&& business_prefix) : business_prefix{business_prefix} {}
                        business_info_page(const business_info_page&) = delete;
                        business_info_page(business_info_page&&) = delete;
                        business_info_page& operator= (const business_info_page&) = delete;
                        business_info_page& operator= (business_info_page&&) = delete;
                        ~business_info_page() = default;

                        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;

                private:
                        [[nodiscard]] bool verify_ui_builder(const Glib::RefPtr<Gtk::Builder>&);
                        void update_business_info_with_db_data(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_wrong_info_alert(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_save_alert(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_save_button(const Glib::RefPtr<Gtk::Builder>&);
                        feature::business extract_page_entries(const Glib::RefPtr<Gtk::Builder>&);

                private:
                        std::string business_prefix{""};
                        Gtk::MessageDialog* wrong_info_alert_dialog{};
                        Gtk::MessageDialog* save_alert_dialog{};
        };
}
#endif
