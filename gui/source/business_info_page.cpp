/***********************************************************
 * Contents: Business Information Page class implementation
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 **********************************************************/
#include <business_info_page.h>

#include <iostream>


bool gui::business_info_page::create(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool created{false};
        if (verify_ui_builder(ui_builder) == true)
        {
                created = true;
                connect_save_button(ui_builder);
        }

        return created;
}

bool gui::business_info_page::verify_ui_builder(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool verified{false};
        if (ui_builder)
        {
                verified = true;
        }

        return verified;
}

void gui::business_info_page::connect_save_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        Gtk::Button* save_button{ui_builder->get_widget<Gtk::Button>(this->business_prefix + "-business-info-save-button")};
        if (save_button)
        {
                save_button->signal_clicked().connect([ui_builder, this] () { save_clicked(ui_builder); });
        }
}

void gui::business_info_page::extract_page_entries(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        Gtk::Entry* name{ui_builder->get_widget<Gtk::Entry>(this->business_prefix + "-business-info-name-entry")};
        Gtk::Entry* street_address{ui_builder->get_widget<Gtk::Entry>(this->business_prefix + "-business-info-address-entry")};
        Gtk::Entry* area_code{ui_builder->get_widget<Gtk::Entry>(this->business_prefix + "-business-info-area-code-entry")};
        Gtk::Entry* town_name{ui_builder->get_widget<Gtk::Entry>(this->business_prefix + "-business-info-town-name-entry")};
        Gtk::Entry* cellphone{ui_builder->get_widget<Gtk::Entry>(this->business_prefix + "-business-info-cell-number-entry")};
        Gtk::Entry* email{ui_builder->get_widget<Gtk::Entry>(this->business_prefix + "-business-info-email-entry")};
        Gtk::Entry* bank_name{ui_builder->get_widget<Gtk::Entry>(this->business_prefix + "-business-info-bank-name-entry")};
        Gtk::Entry* branch_code{ui_builder->get_widget<Gtk::Entry>(this->business_prefix + "-business-info-branch-code-entry")};
        Gtk::Entry* account_number{ui_builder->get_widget<Gtk::Entry>(this->business_prefix + "-business-info-account-number-entry")};
        Gtk::Entry* client_message{ui_builder->get_widget<Gtk::Entry>(this->business_prefix + "-business-info-client-message-entry")};


        this->business_data.set_name(name->get_text());
        this->business_data.set_address(street_address->get_text());
        this->business_data.set_area_code(area_code->get_text());
        this->business_data.set_town(town_name->get_text());
        this->business_data.set_cellphone(cellphone->get_text());
        this->business_data.set_email(email->get_text());
        this->business_data.set_bank(bank_name->get_text());
        this->business_data.set_branch_code(branch_code->get_text());
        this->business_data.set_account_number(account_number->get_text());
        this->business_data.set_client_message(client_message->get_text());
}

void gui::business_info_page::save_clicked(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        extract_page_entries(ui_builder);
        std::cout << "Business Information | Business Name: " << this->business_data.get_name() << std::endl;
        std::cout << "Business Information | Address: " << this->business_data.get_address() << std::endl;
        std::cout << "Business Information | Area Code: " << this->business_data.get_area_code() << std::endl;
        std::cout << "Business Information | Town: " << this->business_data.get_town() << std::endl;
        std::cout << "Business Information | Cellphone: " << this->business_data.get_cellphone() << std::endl;
        std::cout << "Business Information | Email: " << this->business_data.get_email() << std::endl;
        std::cout << "Business Information | Bank Name: " << this->business_data.get_bank() << std::endl;
        std::cout << "Business Information | Branch Code: " << this->business_data.get_branch_code() << std::endl;
        std::cout << "Business Information | Account Number: " << this->business_data.get_account_number() << std::endl;
        std::cout << "Business Information | Client Message: " << this->business_data.get_client_message() << std::endl;
}
