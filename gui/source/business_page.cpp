/***********************************************************
 * Contents: Business Information Page class implementation
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 **********************************************************/
#include <syslog.h>
#include <business_page.h>

gui::business_page::~business_page()
{

}

bool gui::business_page::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        bool created{false};
        if (!_ui_builder)
        {
                syslog(LOG_CRIT, "The _ui_builder is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return created;
        }
        else
        {
                created = true;
                create_entries(_ui_builder);
                update_business_info_with_db_data();
                connect_save_button();
                connect_save_alert();
                connect_wrong_info_alert();
        }

        return created;
}

void gui::business_page::create_entries(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        this->name = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("business-info-name-entry")};
        this->street_address = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("business-info-address-entry")};
        this->area_code = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("business-info-area-code-entry")};
        this->town_name = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("business-info-town-name-entry")};
        this->cellphone = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("business-info-cell-number-entry")};
        this->email = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("business-info-email-entry")};
        this->bank_name = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("business-info-bank-name-entry")};
        this->branch_code = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("business-info-branch-code-entry")};
        this->account_number = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("business-info-account-number-entry")};
        this->client_message = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("business-info-client-message-entry")};
        this->password = std::unique_ptr<Gtk::PasswordEntry>{
                _ui_builder->get_widget<Gtk::PasswordEntry>("business-info-email-password-entry")};
        this->save_button = std::unique_ptr<Gtk::Button>{
                _ui_builder->get_widget<Gtk::Button>("business-info-save-button")};
        this->wrong_info_alert_dialog = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("business-wrong-info-alert")};
        this->save_alert_dialog = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("business-save-button-alert")};
        this->organization_label = std::unique_ptr<Gtk::Label>{
                _ui_builder->get_widget<Gtk::Label>("organization-name")};
}

void gui::business_page::connect_save_button()
{
        if (!this->save_button)
        {
                syslog(LOG_CRIT, "The save_button is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->save_button->signal_clicked().connect([this] () {
                syslog(LOG_INFO, "User clicked the save button - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                this->save_alert_dialog->show();
        });
}

void gui::business_page::connect_save_alert()
{
        if (!this->save_alert_dialog)
        {
                syslog(LOG_CRIT, "The save_alert_dialog is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->save_alert_dialog->signal_response().connect([this] (int response) {
                data::business data{extract_page_entries()};
                switch(response)
                {
                        case GTK_RESPONSE_YES:
                                syslog(LOG_INFO, "User chose to save the business information - "
                                                 "filename %s, line number %d", __FILE__, __LINE__);
                                if (this->business_info.save(data, this->sql) == false)
                                {
                                        syslog(LOG_CRIT, "Failed to save the business information - "
                                                         "filename %s, line number %d", __FILE__, __LINE__);
                                        this->save_alert_dialog->hide();
                                        this->wrong_info_alert_dialog->show();
                                }
                                else
                                {
                                        this->organization_label->set_text(data.get_name());
                                        this->save_alert_dialog->hide();
                                        clear_entries();
                                }
                                break;
                        case GTK_RESPONSE_NO:
                                syslog(LOG_INFO, "User chose not to save the business information - "
                                                 "filename %s, line number %d", __FILE__, __LINE__);
                                this->save_alert_dialog->hide();
                                break;
                        default:
                                this->save_alert_dialog->hide();
                                break;
                }
        });
}

void gui::business_page::connect_wrong_info_alert()
{
        if (!this->wrong_info_alert_dialog)
        {
                syslog(LOG_CRIT, "The wrong_info_alert_dialog is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

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

void gui::business_page::clear_entries()
{
        this->password->set_text("");
}

void gui::business_page::update_business_info_with_db_data()
{
        data::business data{business_info.load(sql)};
        if (data.is_valid() == false)
        {
                syslog(LOG_CRIT, "The business data is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->name->set_text(data.get_name());
        this->street_address->set_text(data.get_address());
        this->area_code->set_text(data.get_area_code());
        this->town_name->set_text(data.get_town());
        this->cellphone->set_text(data.get_cellphone());
        this->email->set_text(data.get_email());
        this->bank_name->set_text(data.get_bank());
        this->branch_code->set_text(data.get_branch_code());
        this->account_number->set_text(data.get_account_number());
        this->client_message->set_text(data.get_client_message());
}

data::business gui::business_page::extract_page_entries()
{
        data::business data{};
        data.set_name(this->name->get_text());
        data.set_address(this->street_address->get_text());
        data.set_area_code(this->area_code->get_text());
        data.set_town(this->town_name->get_text());
        data.set_cellphone(this->cellphone->get_text());
        data.set_email(this->email->get_text());
        data.set_bank(this->bank_name->get_text());
        data.set_branch_code(this->branch_code->get_text());
        data.set_account_number(this->account_number->get_text());
        data.set_client_message(this->client_message->get_text());
        data.set_password(this->password->get_text());

        return data;
}
