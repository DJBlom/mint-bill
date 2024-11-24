/***********************************************************
 * Contents: Business Information Page class implementation
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 **********************************************************/
#include <business_info_page.h>


bool gui::business_info_page::create(const Glib::RefPtr<Gtk::Builder>& ui_builder)
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

bool gui::business_info_page::verify_ui_builder(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool verified{false};
        if (ui_builder)
        {
                verified = true;
        }

        return verified;
}

void gui::business_info_page::update_business_info_with_db_data(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        data::business tmp_data{};
        this->business_feature.load(tmp_data);

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

        name->set_text(tmp_data.get_name());
        street_address->set_text(tmp_data.get_address());
        area_code->set_text(tmp_data.get_area_code());
        town_name->set_text(tmp_data.get_town());
        cellphone->set_text(tmp_data.get_cellphone());
        email->set_text(tmp_data.get_email());
        bank_name->set_text(tmp_data.get_bank());
        branch_code->set_text(tmp_data.get_branch_code());
        account_number->set_text(tmp_data.get_account_number());
        client_message->set_text(tmp_data.get_client_message());
}

void gui::business_info_page::connect_save_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        Gtk::Button* save_button{ui_builder->get_widget<Gtk::Button>(this->business_prefix + "-business-info-save-button")};
        if (save_button)
        {
                save_button->signal_clicked().connect([ui_builder, this] () {
                        this->save_alert_dialog->show();
                });
        }
}

void gui::business_info_page::connect_save_alert(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->save_alert_dialog = ui_builder->get_widget<Gtk::MessageDialog>("save-button-alert");
        static bool save_signal_connected{false};
        if (save_signal_connected == false)
        {
                this->save_alert_dialog->signal_response().connect([ui_builder, this] (int response) {
                        data::business tmp_data = extract_page_entries(ui_builder);
                        switch(response)
                        {
                                case GTK_RESPONSE_YES:
                                        if (this->business_feature.save(tmp_data) == false)
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

                save_signal_connected = true;
        }
}

void gui::business_info_page::connect_wrong_info_alert(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->wrong_info_alert_dialog = ui_builder->get_widget<Gtk::MessageDialog>("wrong-info-alert");
        static bool wrong_info_signal_connected{false};
        if (wrong_info_signal_connected == false)
        {
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

                wrong_info_signal_connected = true;
        }
}

data::business gui::business_info_page::extract_page_entries(const Glib::RefPtr<Gtk::Builder>& ui_builder)
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

        data::business tmp_data{};
        tmp_data.set_name(name->get_text());
        tmp_data.set_address(street_address->get_text());
        tmp_data.set_area_code(area_code->get_text());
        tmp_data.set_town(town_name->get_text());
        tmp_data.set_cellphone(cellphone->get_text());
        tmp_data.set_email(email->get_text());
        tmp_data.set_bank(bank_name->get_text());
        tmp_data.set_branch_code(branch_code->get_text());
        tmp_data.set_account_number(account_number->get_text());
        tmp_data.set_client_message(client_message->get_text());

        return tmp_data;
}
