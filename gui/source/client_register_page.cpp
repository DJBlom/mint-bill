/********************************************************
 * Contents: Client Register Page class implementation
 * Author: Dawid J. Blom
 * Date: November 26, 2024
 *
 * NOTE:
 *******************************************************/
#include <client_register_page.h>
#include <client_data.h>

gui::client_register_page::~client_register_page()
{
}

bool gui::client_register_page::create(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool created{false};
        if (verify_ui_builder(ui_builder) == true)
        {
                created = true;
                connect_search(ui_builder);
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

void gui::client_register_page::connect_search(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::shared_ptr<Gtk::SearchEntry> search_entry{ui_builder->get_widget<Gtk::SearchEntry>("register-search-entry")};
        if (search_entry)
        {
                search_entry->signal_search_changed().connect([search_entry, ui_builder, this] () {
                                data::client data = client_register.search(search_entry->get_text(), this->db);
                                display_on_ui(data, ui_builder);
                });
        }
}

void gui::client_register_page::connect_save_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::Button> save_button{ui_builder->get_widget<Gtk::Button>("register-save-button")};
        if (save_button)
        {
                save_button->signal_clicked().connect([ui_builder, this] () {
                        this->save_alert_dialog->show();
                });
        }
}

data::client extract_page_entries(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::Entry> business_name{ui_builder->get_widget<Gtk::Entry>("register-business-name-entry")};
        std::unique_ptr<Gtk::Entry> business_street_address{ui_builder->get_widget<Gtk::Entry>("register-address-entry")};
        std::unique_ptr<Gtk::Entry> business_area_code{ui_builder->get_widget<Gtk::Entry>("register-area-code-entry")};
        std::unique_ptr<Gtk::Entry> business_town_name{ui_builder->get_widget<Gtk::Entry>("register-town-name-entry")};
        std::unique_ptr<Gtk::Entry> cellphone{ui_builder->get_widget<Gtk::Entry>("register-cell-number-entry")};
        std::unique_ptr<Gtk::Entry> email{ui_builder->get_widget<Gtk::Entry>("register-email-entry")};
        std::unique_ptr<Gtk::Entry> vat_number{ui_builder->get_widget<Gtk::Entry>("register-vat-number-entry")};
        std::unique_ptr<Gtk::Entry> statment_schedule{ui_builder->get_widget<Gtk::Entry>("register-statement-schedule-entry")};

        data::client data{};
        data.set_business_name(business_name->get_text());
        data.set_business_address(business_street_address->get_text());
        data.set_business_area_code(business_area_code->get_text());
        data.set_business_town_name(business_town_name->get_text());
        data.set_cellphone_number(cellphone->get_text());
        data.set_email(email->get_text());
        data.set_vat_number(vat_number->get_text());
        data.set_statement_schedule(statment_schedule->get_text());

        return data;
}

void gui::client_register_page::connect_save_alert(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->save_alert_dialog =
                std::unique_ptr<Gtk::MessageDialog>{ui_builder->get_widget<Gtk::MessageDialog>("client-save-button-alert")};
        this->save_alert_dialog->signal_response().connect([ui_builder, this] (int response) {
                data::client data = extract_page_entries(ui_builder);
                switch(response)
                {
                        case GTK_RESPONSE_YES:
                                if (this->client_register.save(data, this->db) == false)
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

void gui::client_register_page::connect_wrong_info_alert(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->wrong_info_alert_dialog =
                std::unique_ptr<Gtk::MessageDialog>{ui_builder->get_widget<Gtk::MessageDialog>("client-wrong-info-alert")};
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

void gui::client_register_page::display_on_ui(const data::client& data, const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::Entry> business_name{ui_builder->get_widget<Gtk::Entry>("register-business-name-entry")};
        std::unique_ptr<Gtk::Entry> business_street_address{ui_builder->get_widget<Gtk::Entry>("register-address-entry")};
        std::unique_ptr<Gtk::Entry> business_area_code{ui_builder->get_widget<Gtk::Entry>("register-area-code-entry")};
        std::unique_ptr<Gtk::Entry> business_town_name{ui_builder->get_widget<Gtk::Entry>("register-town-name-entry")};
        std::unique_ptr<Gtk::Entry> cellphone{ui_builder->get_widget<Gtk::Entry>("register-cell-number-entry")};
        std::unique_ptr<Gtk::Entry> email{ui_builder->get_widget<Gtk::Entry>("register-email-entry")};
        std::unique_ptr<Gtk::Entry> vat_number{ui_builder->get_widget<Gtk::Entry>("register-vat-number-entry")};
        std::unique_ptr<Gtk::Entry> statment_schedule{ui_builder->get_widget<Gtk::Entry>("register-statement-schedule-entry")};

        business_name->set_text(data.get_business_name());
        business_street_address->set_text(data.get_business_address());
        business_area_code->set_text(data.get_business_area_code());
        business_town_name->set_text(data.get_business_town_name());
        cellphone->set_text(data.get_cellphone_number());

        std::string emails{""};
        for (const auto& email : data.get_email())
                emails += email + " ";
        email->set_text(emails);

        vat_number->set_text(data.get_vat_number());
        statment_schedule->set_text(data.get_statement_schedule());
}
