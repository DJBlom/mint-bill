/********************************************************
 * Contents: Business Information Page class definition
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _BUSINESS_PAGE_H_
#define _BUSINESS_PAGE_H_
#include <gui.h>
#include <sql.h>
#include <business_info.h>

namespace gui {
        //GCOVR_EXCL_START
        class business_page : public interface::gui {
                public:
                        business_page() = default;
                        business_page(const business_page&) = delete;
                        business_page(business_page&&) = delete;
                        business_page& operator= (const business_page&) = delete;
                        business_page& operator= (business_page&&) = delete;
                        ~business_page() override;

			[[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&, const interface::search&) override;

                private:
                        void create_entries(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_wrong_info_alert();
                        void connect_save_alert();
                        void connect_save_button();
                        void clear_entries();
                        void update_business_info_with_db_data(const std::string&);
                        data::business extract_page_entries();

                private:
                        storage::sql sql{};
                        feature::business business_info{};

                private: // Label
                        std::unique_ptr<Gtk::Label> organization_label{};

                private: // Entries
                        std::unique_ptr<Gtk::Entry> name{};
                        std::unique_ptr<Gtk::Entry> street_address{};
                        std::unique_ptr<Gtk::Entry> area_code{};
                        std::unique_ptr<Gtk::Entry> town_name{};
                        std::unique_ptr<Gtk::Entry> cellphone{};
                        std::unique_ptr<Gtk::Entry> email{};
                        std::unique_ptr<Gtk::Entry> bank_name{};
                        std::unique_ptr<Gtk::Entry> branch_code{};
                        std::unique_ptr<Gtk::Entry> account_number{};
                        std::unique_ptr<Gtk::Entry> client_message{};
                        std::unique_ptr<Gtk::PasswordEntry> password{};
                        std::unique_ptr<Gtk::Button> save_button{};

                private: // Dialogs
                        std::unique_ptr<Gtk::MessageDialog> wrong_info_alert_dialog{};
                        std::unique_ptr<Gtk::MessageDialog> save_alert_dialog{};
        };

        //GCOVR_EXCL_STOP
}
#endif
