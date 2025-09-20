/***********************************************************
 * Contents: admin Information Page class implementation
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 **********************************************************/
#include <syslog.h>
#include <admin_page.h>
//GCOVR_EXCL_START
gui::admin_page::~admin_page()
{

}

bool gui::admin_page::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        bool created{true};
        if (!_ui_builder)
        {
                syslog(LOG_CRIT, "The _ui_builder is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		created = false;
        }
        else
        {
                create_entries(_ui_builder);
                connect_save_alert();
                connect_wrong_info_alert();
        }

        return created;
}

bool gui::admin_page::search(const std::string& _keyword)
{
        bool searched{false};
        if (_keyword.empty())
        {
                syslog(LOG_CRIT, "The _keywword is empty - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
		searched = true;
		update_business_info_with_db_data(_keyword);
        }

        return searched;
}

bool gui::admin_page::save()
{
	bool success{false};
	if (!this->save_alert_dialog)
	{
                syslog(LOG_CRIT, "The save_alert_dialog is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->save_alert_dialog->show();
	}

	return success;
}

void gui::admin_page::create_entries(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        this->name = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("admin-page-name-entry")};
        this->street_address = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("admin-page-address-entry")};
        this->area_code = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("admin-page-area-code-entry")};
        this->town_name = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("admin-page-town-name-entry")};
        this->cellphone = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("admin-page-cell-number-entry")};
        this->email = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("admin-page-email-entry")};
        this->bank_name = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("admin-page-bank-name-entry")};
        this->branch_code = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("admin-page-branch-code-entry")};
        this->account_number = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("admin-page-account-number-entry")};
        this->client_message = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("admin-page-client-message-entry")};
        this->password = std::unique_ptr<Gtk::PasswordEntry>{
                _ui_builder->get_widget<Gtk::PasswordEntry>("admin-page-email-password-entry")};
        this->wrong_info_alert_dialog = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("admin-wrong-info-alert")};
        this->save_alert_dialog = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("admin-save-button-alert")};
        this->organization_label = std::unique_ptr<Gtk::Label>{
                _ui_builder->get_widget<Gtk::Label>("organization-name")};
}

void gui::admin_page::connect_save_alert()
{
        if (!this->save_alert_dialog)
        {
                syslog(LOG_CRIT, "The save_alert_dialog is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->save_alert_dialog->signal_response().connect([this] (int response) {
                data::admin data{extract_page_entries()};
                switch(response)
                {
                        case GTK_RESPONSE_YES:
                                syslog(LOG_INFO, "User chose to save the admin information - "
                                                 "filename %s, line number %d", __FILE__, __LINE__);
                                if (this->admin_model.save(data) == false)
                                {
                                        syslog(LOG_CRIT, "Failed to save the admin information - "
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
                                syslog(LOG_INFO, "User chose not to save the admin information - "
                                                 "filename %s, line number %d", __FILE__, __LINE__);
                                this->save_alert_dialog->hide();
                                break;
                        default:
                                this->save_alert_dialog->hide();
                                break;
                }
        });
}

void gui::admin_page::connect_wrong_info_alert()
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

void gui::admin_page::clear_entries()
{
        this->password->set_text("");
}

void gui::admin_page::update_business_info_with_db_data(const std::string& _keyword)
{
	(void) _keyword;
	std::any data{admin_model.load("tme")};
	data::admin business_data{std::any_cast<data::admin> (data)};
        if (business_data.is_valid() == false)
        {
                syslog(LOG_CRIT, "The admin data is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->name->set_text(business_data.get_name());
        this->street_address->set_text(business_data.get_address());
        this->area_code->set_text(business_data.get_area_code());
        this->town_name->set_text(business_data.get_town());
        this->cellphone->set_text(business_data.get_cellphone());
        this->email->set_text(business_data.get_email());
        this->bank_name->set_text(business_data.get_bank());
        this->branch_code->set_text(business_data.get_branch_code());
        this->account_number->set_text(business_data.get_account_number());
        this->client_message->set_text(business_data.get_client_message());
}

data::admin gui::admin_page::extract_page_entries()
{
        data::admin data{};
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
//GCOVR_EXCL_STOP
