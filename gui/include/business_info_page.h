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
#include <iostream>

namespace gui {
        class business_info_page : public interface::gui {
                public:
                        business_info_page() = default;
                        business_info_page(const business_info_page&) = delete;
                        business_info_page(business_info_page&&) = delete;
                        business_info_page& operator= (const business_info_page&) = delete;
                        business_info_page& operator= (business_info_page&&) = delete;
                        ~business_info_page() = default;

                        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;

                private:
                        void connect_save_button(const Glib::RefPtr<Gtk::Builder>&);
                        void save_clicked();

                private:
                        Gtk::Entry* business_name;
        };
}
#endif
