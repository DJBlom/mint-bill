/********************************************************
 * Contents: Client Register Page class implementation
 * Author: Dawid J. Blom
 * Date: November 26, 2024
 *
 * NOTE:
 *******************************************************/
#include <client_register_page.h>

gui::client_register_page::~client_register_page()
{
}

bool gui::client_register_page::create(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool created{false};
        if (verify_ui_builder(ui_builder) == true)
        {
                created = true;
                connect_save_button(ui_builder);
                connect_save_alert(ui_builder);
                connect_wrong_info_alert(ui_builder);

        }
        return created;
}

bool gui::client_register_page::verify_ui_builder(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool verified{false};
        if (ui_builder)
        {
                verified = true;
        }

        return verified;
}

void gui::client_register_page::connect_save_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        Gtk::Button* save_button{ui_builder->get_widget<Gtk::Button>("register-save-button")};
        if (save_button)
        {
                save_button->signal_clicked().connect([ui_builder, this] () {
                        this->save_alert_dialog->show();
                });
        }
}

void gui::client_register_page::connect_save_alert(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->save_alert_dialog = ui_builder->get_widget<Gtk::MessageDialog>("client-save-button-alert");
        this->save_alert_dialog->signal_response().connect([ui_builder, this] (int response) {
                switch(response)
                {
                        case GTK_RESPONSE_YES:
                                this->save_alert_dialog->hide();
                                this->wrong_info_alert_dialog->show();
                                break;
                        case GTK_RESPONSE_NO:
                                this->save_alert_dialog->hide();
                                break;
                        default:
                                this->save_alert_dialog->hide();
                                break;
                }
        });
}

void gui::client_register_page::connect_wrong_info_alert(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->wrong_info_alert_dialog = ui_builder->get_widget<Gtk::MessageDialog>("client-wrong-info-alert");
        this->wrong_info_alert_dialog->signal_response().connect([this] (int response) {
                switch (response)
                {
                        case GTK_RESPONSE_CLOSE:
                                this->wrong_info_alert_dialog->hide();
                                break;
                        default:
                                this->wrong_info_alert_dialog->hide();
                                break;
                }
        });
}

//data::business gui::client_register_page::extract_page_entries(const Glib::RefPtr<Gtk::Builder>& ui_builder)
//{
//        Gtk::Entry* name{ui_builder->get_widget<Gtk::Entry>("business-info-name-entry")};
//        Gtk::Entry* street_address{ui_builder->get_widget<Gtk::Entry>("business-info-address-entry")};
//        Gtk::Entry* area_code{ui_builder->get_widget<Gtk::Entry>("business-info-area-code-entry")};
//        Gtk::Entry* town_name{ui_builder->get_widget<Gtk::Entry>("business-info-town-name-entry")};
//        Gtk::Entry* cellphone{ui_builder->get_widget<Gtk::Entry>("business-info-cell-number-entry")};
//        Gtk::Entry* email{ui_builder->get_widget<Gtk::Entry>("business-info-email-entry")};
//        Gtk::Entry* bank_name{ui_builder->get_widget<Gtk::Entry>("business-info-bank-name-entry")};
//        Gtk::Entry* branch_code{ui_builder->get_widget<Gtk::Entry>("business-info-branch-code-entry")};
//        Gtk::Entry* account_number{ui_builder->get_widget<Gtk::Entry>("business-info-account-number-entry")};
//        Gtk::Entry* client_message{ui_builder->get_widget<Gtk::Entry>("business-info-client-message-entry")};
//
//        data::business data{};
//        data.set_name(name->get_text());
//        data.set_address(street_address->get_text());
//        data.set_area_code(area_code->get_text());
//        data.set_town(town_name->get_text());
//        data.set_cellphone(cellphone->get_text());
//        data.set_email(email->get_text());
//        data.set_bank(bank_name->get_text());
//        data.set_branch_code(branch_code->get_text());
//        data.set_account_number(account_number->get_text());
//        data.set_client_message(client_message->get_text());
//
//        return data;
//}
