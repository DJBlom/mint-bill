/********************************************************
 * Contents: Client Register Page class implementation
 * Author: Dawid J. Blom
 * Date: November 26, 2024
 *
 * NOTE:
 *******************************************************/
#include <syslog.h>
#include <client_data.h>
#include <client_register_page.h>

//GCOVR_EXCL_START
gui::client_register_page::~client_register_page()
{
}

bool gui::client_register_page::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder,
				       const interface::search& _search_bar)
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
		created = _search_bar.subscribe("register-page", [this] (const std::string& _keyword) {
			data::client data = this->client_register.search(_keyword, this->db);
			display_on_ui(data);
		});
                create_entries(_ui_builder);
                connect_save_button();
                connect_save_alert();
                connect_wrong_info_alert();
        }

        return created;
}

void gui::client_register_page::create_entries(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        this->email = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("register-email-entry")};
        this->cellphone = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("register-cell-number-entry")};
        this->vat_number = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("register-vat-number-entry")};
        this->business_name = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("register-business-name-entry")};
        this->statment_schedule = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("register-statement-schedule-entry")};
        this->business_area_code = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("register-area-code-entry")};
        this->business_town_name = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("register-town-name-entry")};
        this->business_street_address = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("register-address-entry")};
        this->save_button = std::unique_ptr<Gtk::Button>{
                _ui_builder->get_widget<Gtk::Button>("register-save-button")};
        this->save_alert_dialog = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("client-save-button-alert")};
        this->wrong_info_alert_dialog = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("client-wrong-info-alert")};
}

void gui::client_register_page::connect_save_button()
{
        if (!this->save_button)
        {
                syslog(LOG_CRIT, "The save_button is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->save_button->signal_clicked().connect([this] () {
                this->save_alert_dialog->show();
        });
}

void gui::client_register_page::connect_save_alert()
{
        if (!this->save_alert_dialog)
        {
                syslog(LOG_CRIT, "The save_alert_dialog is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->save_alert_dialog->signal_response().connect([this] (int response) {
                data::client data = extract_page_entries();
                switch(response)
                {
                        case GTK_RESPONSE_YES:
                                syslog(LOG_INFO, "User chose to save the client information - "
                                                 "filename %s, line number %d", __FILE__, __LINE__);
                                if (this->client_register.save(data, this->db) == false)
                                {
                                        this->save_alert_dialog->hide();
                                        this->wrong_info_alert_dialog->show();
                                }
                                else
                                {
                                        this->save_alert_dialog->hide();
                                        clear_all_entries();
                                }
                                break;
                        case GTK_RESPONSE_NO:
                                syslog(LOG_INFO, "User chose not to save the client information - "
                                                 "filename %s, line number %d", __FILE__, __LINE__);
                                this->save_alert_dialog->hide();
                                break;
                        default:
                                this->save_alert_dialog->hide();
                                break;
                }
        });
}

void gui::client_register_page::connect_wrong_info_alert()
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

void gui::client_register_page::clear_all_entries()
{
        this->search_entry->set_text("");
        this->business_name->set_text("");
        this->business_street_address->set_text("");
        this->business_area_code->set_text("");
        this->business_town_name->set_text("");
        this->cellphone->set_text("");
        this->email->set_text("");
        this->vat_number->set_text("");
        this->statment_schedule->set_text("");
}

void gui::client_register_page::display_on_ui(const data::client& _data)
{
        this->email->set_text(_data.get_email());
        this->vat_number->set_text(_data.get_vat_number());
        this->cellphone->set_text(_data.get_cellphone_number());
        this->business_name->set_text(_data.get_business_name());
        this->statment_schedule->set_text(_data.get_statement_schedule());
        this->business_area_code->set_text(_data.get_business_area_code());
        this->business_town_name->set_text(_data.get_business_town_name());
        this->business_street_address->set_text(_data.get_business_address());
}

data::client gui::client_register_page::extract_page_entries()
{
        data::client data{};
        data.set_email(this->email->get_text());
        data.set_vat_number(this->vat_number->get_text());
        data.set_cellphone_number(this->cellphone->get_text());
        data.set_business_name(this->business_name->get_text());
        data.set_statement_schedule(this->statment_schedule->get_text());
        data.set_business_area_code(this->business_area_code->get_text());
        data.set_business_town_name(this->business_town_name->get_text());
        data.set_business_address(this->business_street_address->get_text());

        return data;
}

//GCOVR_EXCL_STOP
