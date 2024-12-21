/***********************************************************
 * Contents: Business Information Page class implementation
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 **********************************************************/
#include <business_page.h>


gui::business_page::~business_page()
{
}

bool gui::business_page::create(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool created{false};
        if (verify_ui_builder(ui_builder) == true)
        {
                created = true;
                update_business_info_with_db_data(ui_builder);
                connect_save_button(ui_builder);
                connect_save_alert(ui_builder);
                connect_wrong_info_alert(ui_builder);
        }

        return created;
}

bool gui::business_page::verify_ui_builder(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool verified{false};
        if (ui_builder)
        {
                verified = true;
        }

        return verified;
}

void gui::business_page::update_business_info_with_db_data(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::Entry> name{ui_builder->get_widget<Gtk::Entry>("business-info-name-entry")};
        std::unique_ptr<Gtk::Entry> street_address{ui_builder->get_widget<Gtk::Entry>("business-info-address-entry")};
        std::unique_ptr<Gtk::Entry> area_code{ui_builder->get_widget<Gtk::Entry>("business-info-area-code-entry")};
        std::unique_ptr<Gtk::Entry> town_name{ui_builder->get_widget<Gtk::Entry>("business-info-town-name-entry")};
        std::unique_ptr<Gtk::Entry> cellphone{ui_builder->get_widget<Gtk::Entry>("business-info-cell-number-entry")};
        std::unique_ptr<Gtk::Entry> email{ui_builder->get_widget<Gtk::Entry>("business-info-email-entry")};
        std::unique_ptr<Gtk::Entry> bank_name{ui_builder->get_widget<Gtk::Entry>("business-info-bank-name-entry")};
        std::unique_ptr<Gtk::Entry> branch_code{ui_builder->get_widget<Gtk::Entry>("business-info-branch-code-entry")};
        std::unique_ptr<Gtk::Entry> account_number{ui_builder->get_widget<Gtk::Entry>("business-info-account-number-entry")};
        std::unique_ptr<Gtk::Entry> client_message{ui_builder->get_widget<Gtk::Entry>("business-info-client-message-entry")};

        data::business data{business_info.load(sql)};
        name->set_text(data.get_name());
        street_address->set_text(data.get_address());
        area_code->set_text(data.get_area_code());
        town_name->set_text(data.get_town());
        cellphone->set_text(data.get_cellphone());
        email->set_text(data.get_email());
        bank_name->set_text(data.get_bank());
        branch_code->set_text(data.get_branch_code());
        account_number->set_text(data.get_account_number());
        client_message->set_text(data.get_client_message());
}

void gui::business_page::connect_save_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        Gtk::Button* save_button{ui_builder->get_widget<Gtk::Button>("business-info-save-button")};
        if (save_button)
        {
                save_button->signal_clicked().connect([ui_builder, this] () {
                        this->save_alert_dialog->show();
                });
        }
}

void gui::business_page::connect_save_alert(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->save_alert_dialog = ui_builder->get_widget<Gtk::MessageDialog>("business-save-button-alert");
        this->save_alert_dialog->signal_response().connect([ui_builder, this] (int response) {
                data::business data = extract_page_entries(ui_builder);
                switch(response)
                {
                        case GTK_RESPONSE_YES:
                                if (this->business_info.save(data, this->sql) == false)
                                {
                                        this->save_alert_dialog->hide();
                                        this->wrong_info_alert_dialog->show();
                                }
                                else
                                {
                                        this->save_alert_dialog->hide();
                                }
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

void gui::business_page::connect_wrong_info_alert(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->wrong_info_alert_dialog = ui_builder->get_widget<Gtk::MessageDialog>("business-wrong-info-alert");
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

data::business gui::business_page::extract_page_entries(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::Entry> name{ui_builder->get_widget<Gtk::Entry>("business-info-name-entry")};
        std::unique_ptr<Gtk::Entry> street_address{ui_builder->get_widget<Gtk::Entry>("business-info-address-entry")};
        std::unique_ptr<Gtk::Entry> area_code{ui_builder->get_widget<Gtk::Entry>("business-info-area-code-entry")};
        std::unique_ptr<Gtk::Entry> town_name{ui_builder->get_widget<Gtk::Entry>("business-info-town-name-entry")};
        std::unique_ptr<Gtk::Entry> cellphone{ui_builder->get_widget<Gtk::Entry>("business-info-cell-number-entry")};
        std::unique_ptr<Gtk::Entry> email{ui_builder->get_widget<Gtk::Entry>("business-info-email-entry")};
        std::unique_ptr<Gtk::Entry> bank_name{ui_builder->get_widget<Gtk::Entry>("business-info-bank-name-entry")};
        std::unique_ptr<Gtk::Entry> branch_code{ui_builder->get_widget<Gtk::Entry>("business-info-branch-code-entry")};
        std::unique_ptr<Gtk::Entry> account_number{ui_builder->get_widget<Gtk::Entry>("business-info-account-number-entry")};
        std::unique_ptr<Gtk::Entry> client_message{ui_builder->get_widget<Gtk::Entry>("business-info-client-message-entry")};
        std::unique_ptr<Gtk::PasswordEntry> password{ui_builder->get_widget<Gtk::PasswordEntry>("business-info-email-password-entry")};

        data::business data{};
        data.set_name(name->get_text());
        data.set_address(street_address->get_text());
        data.set_area_code(area_code->get_text());
        data.set_town(town_name->get_text());
        data.set_cellphone(cellphone->get_text());
        data.set_email(email->get_text());
        data.set_bank(bank_name->get_text());
        data.set_branch_code(branch_code->get_text());
        data.set_account_number(account_number->get_text());
        data.set_client_message(client_message->get_text());
        data.set_password(password->get_text());

        return data;
}
