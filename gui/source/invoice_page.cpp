/***********************************************************
 * Contents: Invoice Page class implementation
 * Author: Dawid J. Blom
 * Date: November 28, 2024
 *
 * NOTE:
 **********************************************************/
#include <syslog.h>
#include <invoice_page.h>
#include <iostream>

namespace limit {
        constexpr std::uint8_t MAX_QUANTITY{9};
        constexpr std::uint8_t MAX_AMOUNT{10};
        constexpr std::uint16_t MAX_DESCRIPTION{500};
}

gui::invoice_page::invoice_page()
{
        this->email_dispatcher.connect(sigc::mem_fun(*this, &invoice_page::email_sent));
        this->print_dispatcher.connect(sigc::mem_fun(*this, &invoice_page::printed));
}

gui::invoice_page::~invoice_page()
{

}

bool gui::invoice_page::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        bool created{false};
        if (!_ui_builder)
        {
                syslog(LOG_CRIT, "UI builder is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return created;
        }
        else
        {
                created = true;
                create_views(_ui_builder);
                create_entries(_ui_builder);
                create_dialogs(_ui_builder);
                create_buttons(_ui_builder);
                connect_search();
                connect_save_button();
                connect_material_view();
                connect_description_view();
                connect_save_alert();
                connect_wrong_info_alert();
                connect_wrong_data_in_amount_column_alert();
                connect_wrong_data_in_quantity_column_alert();
                setup_page();
                connect_email_alert();
                connect_email_button();
                connect_print_alert();
                connect_print_button();
                connect_invoice_view();
                connect_no_printer_alert();
                connect_no_internet_alert();
        }

        return created;
}

void gui::invoice_page::create_views(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        this->description_view = std::unique_ptr<Gtk::ColumnView>{
                _ui_builder->get_widget<Gtk::ColumnView>("new-invoice-description")};
        this->description_adjustment = std::shared_ptr<Gtk::Adjustment>{
                _ui_builder->get_object<Gtk::Adjustment>("new-invoice-description-column-view-vadjustment")};
        this->material_view = std::unique_ptr<Gtk::ColumnView>{
                _ui_builder->get_widget<Gtk::ColumnView>("new-invoice-material")};
        this->material_adjustment = std::shared_ptr<Gtk::Adjustment>{
                _ui_builder->get_object<Gtk::Adjustment>("new-invoice-material-column-view-adjustment")};
        this->invoice_view = std::unique_ptr<Gtk::ListView>{
                _ui_builder->get_widget<Gtk::ListView>("known-invoice-view")};
        this->invoices_adjustment = std::shared_ptr<Gtk::Adjustment>{
                _ui_builder->get_object<Gtk::Adjustment>("known-invoice-adjustment")};
}

void gui::invoice_page::create_entries(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        this->job_card = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("new-invoice-job-card-entry")};
        this->order_number = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("new-invoice-order-number-entry")};
        this->description_total_label = std::unique_ptr<Gtk::Label>{
                _ui_builder->get_widget<Gtk::Label>("new-invoice-description-total-label")};
        this->material_total_label = std::unique_ptr<Gtk::Label>{
                _ui_builder->get_widget<Gtk::Label>("new-invoice-material-total-label")};
        this->grand_total_label = std::unique_ptr<Gtk::Label>{
                _ui_builder->get_widget<Gtk::Label>("new-invoice-total-label")};
        this->invoice_date = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("new-invoice-date-entry")};
        this->invoice_number = std::unique_ptr<Gtk::Entry>{
                _ui_builder->get_widget<Gtk::Entry>("new-invoice-number-entry")};
        this->search_entry = std::unique_ptr<Gtk::SearchEntry>{
                _ui_builder->get_widget<Gtk::SearchEntry>("invoice-search-entry")};
}

void gui::invoice_page::create_dialogs(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        this->save_alert_dialog = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-save-button-alert")};
        this->wrong_info_alert_dialog = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-wrong-info-alert")};
        this->wrong_data_in_quantity_column = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-data-in-quantity-column-alert")};
        this->wrong_data_in_amount_column = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-data-in-amount-column-alert")};
        this->email_no_internet = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-email-no-internet-alert")};
        this->email_confirmation = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-email-alert")};
        this->print_confirmation = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-print-alert")};
        this->print_no_printer = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-print-no-printer-alert")};
}

void gui::invoice_page::create_buttons(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        this->save_button = std::unique_ptr<Gtk::Button>{
                _ui_builder->get_widget<Gtk::Button>("new-invoice-save-button")};
        this->description_add_button = std::unique_ptr<Gtk::Button>{
                _ui_builder->get_widget<Gtk::Button>("invoice-description-add-button")};
        this->material_add_button = std::unique_ptr<Gtk::Button>{
                _ui_builder->get_widget<Gtk::Button>("invoice-material-button")};
        this->material_delete_button = std::unique_ptr<Gtk::Button>{
                _ui_builder->get_widget<Gtk::Button>("invoice-material-delete-button")};
        this->description_delete_button = std::unique_ptr<Gtk::Button>{
                _ui_builder->get_widget<Gtk::Button>("invoice-description-delete-button")};
        this->print_button = std::unique_ptr<Gtk::Button>{
                _ui_builder->get_widget<Gtk::Button>("known-invoice-print-button")};
        this->email_button = std::unique_ptr<Gtk::Button>{
                _ui_builder->get_widget<Gtk::Button>("known-invoice-email-button")};
}

void gui::invoice_page::perform_search() {
        std::string business_name{this->search_entry->get_text()};
        this->populate(business_name);
        this->invoice_number->set_text("51");
        this->invoice_date->set_text("10-12-2024");
}

void gui::invoice_page::on_search_changed() {
    Glib::signal_timeout().connect_once(
        sigc::mem_fun(*this, &invoice_page::perform_search), 500, Glib::PRIORITY_DEFAULT);
}

void gui::invoice_page::connect_search()
{
        if (!this->search_entry)
        {
                syslog(LOG_CRIT, "The search_entry is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        search_entry->signal_changed().connect(sigc::mem_fun(*this, &invoice_page::on_search_changed));
}

void gui::invoice_page::connect_save_button()
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

void gui::invoice_page::connect_description_view()
{
        this->description_store = std::shared_ptr<Gio::ListStore<column_entries>>{
                Gio::ListStore<column_entries>::create()};
        if (!this->description_store)
        {
                syslog(LOG_CRIT, "The description_store is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        Glib::RefPtr<Gtk::MultiSelection> selection_model = Gtk::MultiSelection::create(this->description_store);
        if (!selection_model)
        {
                syslog(LOG_CRIT, "The description selection_model is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->description_view->set_model(selection_model);
        this->description_view->set_name("description_view");

        connect_description_add_button();
        connect_description_delete_button(selection_model);
        connect_description_list_store();

        quantity_column(this->description_view);
        description_column(this->description_view);
        amount_column(this->description_view);
}

void gui::invoice_page::connect_description_add_button()
{
        if (!this->description_add_button)
        {
                syslog(LOG_CRIT, "The description_add_button is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->description_add_button->signal_clicked().connect([this] () {
                syslog(LOG_INFO, "User is adding a description row - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                this->description_store->append(column_entries::create());
                Glib::signal_timeout().connect_once([this]() {
                        this->description_adjustment->set_value(this->description_adjustment->get_upper());
                }, 30);
        });
}

void gui::invoice_page::connect_description_delete_button(const Glib::RefPtr<Gtk::MultiSelection>& selection_model)
{
        if (!this->description_delete_button)
        {
                syslog(LOG_CRIT, "The description_delete_button is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->description_delete_button->signal_clicked().connect([selection_model, this] () {
                syslog(LOG_INFO, "User is removing description row - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                Glib::RefPtr<const Gtk::Bitset> indices = selection_model->get_selection();
                while (indices->get_size() != 0)
                {
                        this->description_store->remove(indices->get_minimum());
                }
        });
}

void gui::invoice_page::connect_description_list_store()
{
        this->description_store->signal_items_changed().connect(
                        sigc::mem_fun(*this, &invoice_page::update_description_total));
}

void gui::invoice_page::edit_known_invoice(uint position)
{
        auto item = this->invoice_store->get_item(position);
        if (!item)
        {
                syslog(LOG_CRIT, "The item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        auto data = std::dynamic_pointer_cast<invoice_entries>(item);
        if (!data)
        {
                syslog(LOG_CRIT, "The data is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        data::invoice invoice{data->invoice};
        if (invoice.is_valid() == false)
        {
                syslog(LOG_CRIT, "The invoice data is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        syslog(LOG_INFO, "User is editing an invoice - "
                         "filename %s, line number %d", __FILE__, __LINE__);
        this->invoice_number->set_text(invoice.get_invoice_number());
        this->invoice_date->set_text(invoice.get_invoice_date());
        this->job_card->set_text(invoice.get_job_card_number());
        this->order_number->set_text(invoice.get_order_number());
        this->description_total_label->set_text("Total: R " + invoice.get_description_total());
        this->material_total_label->set_text("Total: R " + invoice.get_material_total());
        this->grand_total_label->set_text("Grand Total: R " + invoice.get_grand_total());
        populate_description_store(invoice);
        populate_material_store(invoice);
}

void gui::invoice_page::update_description_total(uint position, uint removed, uint added)
{
        position++;
        removed++;
        added++;

        std::ostringstream doss{""};
        doss << std::fixed << std::setprecision(2) << compute_total(this->description_store);
        this->description_total_label->set_text("Total: R " + doss.str());
        this->description_total = doss.str();

        std::ostringstream moss{""};
        moss << std::fixed << std::setprecision(2) << compute_total(this->material_store);
        this->material_total_label->set_text("Total: R " + moss.str());
        this->material_total = moss.str();

        std::ostringstream goss{""};
        goss << std::fixed << std::setprecision(2) << this->compute_grand_total();
        this->grand_total_label->set_text("Grand Total: R " + goss.str());
        this->grand_total = goss.str();
}

void gui::invoice_page::connect_material_view()
{
        this->material_store = std::shared_ptr<Gio::ListStore<column_entries>>{
                Gio::ListStore<column_entries>::create()};
        if (!this->material_store)
        {
                syslog(LOG_CRIT, "The material_store is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        Glib::RefPtr<Gtk::MultiSelection> selection_model = Gtk::MultiSelection::create(this->material_store);
        if (!selection_model)
        {
                syslog(LOG_CRIT, "The material selection_model is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->material_view->set_model(selection_model);
        this->material_view->set_name("material_view");

        connect_material_add_button();
        connect_material_delete_button(selection_model);
        connect_material_list_store();

        quantity_column(this->material_view);
        description_column(this->material_view);
        amount_column(this->material_view);
}

void gui::invoice_page::connect_material_add_button()
{
        if (!this->material_add_button)
        {
                syslog(LOG_CRIT, "The material_add_button is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->material_add_button->signal_clicked().connect([this] () {
                syslog(LOG_INFO, "User is adding a material row - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                this->material_store->append(column_entries::create());
                Glib::signal_timeout().connect_once([this]() {
                        this->material_adjustment->set_value(this->material_adjustment->get_upper());
                }, 30);
        });
}

void gui::invoice_page::connect_material_delete_button(const Glib::RefPtr<Gtk::MultiSelection>& selection_model)
{
        if (!this->material_delete_button)
        {
                syslog(LOG_CRIT, "The material_delete_button is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->material_delete_button->signal_clicked().connect([selection_model, this] () {
                syslog(LOG_INFO, "User is removing a material row - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                Glib::RefPtr<const Gtk::Bitset> indices = selection_model->get_selection();
                while (indices->get_size() != 0)
                {
                        this->material_store->remove(indices->get_minimum());
                }
        });
}

void gui::invoice_page::connect_material_list_store()
{
        this->material_store->signal_items_changed().connect(
                        sigc::mem_fun(*this, &invoice_page::update_material_total));
}

void gui::invoice_page::update_material_total(uint position, uint removed, uint added)
{
        position++;
        removed++;
        added++;

        std::ostringstream doss{""};
        doss << std::fixed << std::setprecision(2) << compute_total(this->description_store);
        this->description_total_label->set_text("Total: R " + doss.str());
        this->description_total = doss.str();

        std::ostringstream moss{""};
        moss << std::fixed << std::setprecision(2) << compute_total(this->material_store);
        this->material_total_label->set_text("Total: R " + moss.str());
        this->material_total = moss.str();

        std::ostringstream goss{""};
        goss << std::fixed << std::setprecision(2) << this->compute_grand_total();
        this->grand_total_label->set_text("Grand Total: R " + goss.str());
        this->grand_total = goss.str();
}

void gui::invoice_page::connect_save_alert()
{
        if (!this->save_alert_dialog)
        {
                syslog(LOG_CRIT, "The save_alert_dialog is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->save_alert_dialog->signal_response().connect([this] (int response) {
                data::invoice data{this->extract_invoice_data()};
                switch(response)
                {
                        case GTK_RESPONSE_YES:
                                syslog(LOG_INFO, "User chose to save the information - "
                                                 "filename %s, line number %d", __FILE__, __LINE__);
                                if (this->client_invoice.save(data, this->db) == false)
                                {
                                        syslog(LOG_CRIT, "Failed to save the invoice information - "
                                                         "filename %s, line number %d", __FILE__, __LINE__);
                                        this->save_alert_dialog->hide();
                                        this->wrong_info_alert_dialog->show();
                                }
                                else
                                {
                                        this->add(data);
                                        this->save_alert_dialog->hide();
                                        this->description_store->remove_all();
                                        this->material_store->remove_all();
                                }
                                break;
                        case GTK_RESPONSE_NO:
                                syslog(LOG_INFO, "User chose not to save the information - "
                                                 "filename %s, line number %d", __FILE__, __LINE__);
                                this->save_alert_dialog->hide();
                                break;
                        default:
                                this->save_alert_dialog->hide();
                                break;
                }
        });
}

void gui::invoice_page::connect_wrong_info_alert()
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

void gui::invoice_page::connect_wrong_data_in_amount_column_alert()
{
        if (!this->wrong_data_in_amount_column)
        {
                syslog(LOG_CRIT, "The wrong_data_in_amount_column is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->wrong_data_in_amount_column->signal_response().connect([this] (int response) {
                switch (response)
                {
                        case GTK_RESPONSE_CLOSE:
                                this->wrong_data_in_amount_column->hide();
                                break;
                        default:
                                this->wrong_data_in_amount_column->hide();
                                break;
                }
        });
}

void gui::invoice_page::connect_wrong_data_in_quantity_column_alert()
{
        if (!this->wrong_data_in_quantity_column)
        {
                syslog(LOG_CRIT, "The wrong_data_in_quantity_column is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->wrong_data_in_quantity_column->signal_response().connect([this] (int response) {
                switch (response)
                {
                        case GTK_RESPONSE_CLOSE:
                                this->wrong_data_in_quantity_column->hide();
                                break;
                        default:
                                this->wrong_data_in_quantity_column->hide();
                                break;
                }
        });
}

void gui::invoice_page::quantity_column(const std::unique_ptr<Gtk::ColumnView>& view)
{
        if (!view)
        {
                syslog(LOG_CRIT, "The view is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        Glib::RefPtr<Gtk::SignalListItemFactory> factory = Gtk::SignalListItemFactory::create();
        if (!factory)
        {
                syslog(LOG_CRIT, "The factory is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        Glib::RefPtr<Gtk::ColumnViewColumn> column = Gtk::ColumnViewColumn::create("Quantity", factory);
        if (!column)
        {
                syslog(LOG_CRIT, "The column is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::setup)));
        factory->signal_bind().connect(sigc::mem_fun(*this, &invoice_page::bind_quantity));
        factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::teardown)));
        column->set_expand(false);
        view->append_column(column);
}

void gui::invoice_page::description_column(const std::unique_ptr<Gtk::ColumnView>& view)
{
        if (!view)
        {
                syslog(LOG_CRIT, "The view is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        Glib::RefPtr<Gtk::SignalListItemFactory> factory = Gtk::SignalListItemFactory::create();
        if (!factory)
        {
                syslog(LOG_CRIT, "The factory is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        Glib::RefPtr<Gtk::ColumnViewColumn> column = Gtk::ColumnViewColumn::create("Description", factory);
        if (!column)
        {
                syslog(LOG_CRIT, "The column is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::setup)));
        factory->signal_bind().connect(sigc::mem_fun(*this, &invoice_page::bind_description));
        factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::teardown)));
        column->set_expand(true);
        view->append_column(column);
}

void gui::invoice_page::amount_column(const std::unique_ptr<Gtk::ColumnView>& view)
{
        if (!view)
        {
                syslog(LOG_CRIT, "The view is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        Glib::RefPtr<Gtk::SignalListItemFactory> factory = Gtk::SignalListItemFactory::create();
        if (!factory)
        {
                syslog(LOG_CRIT, "The factory is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        Glib::RefPtr<Gtk::ColumnViewColumn> column = Gtk::ColumnViewColumn::create("Amount", factory);
        if (!column)
        {
                syslog(LOG_CRIT, "The column is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::setup)));
        factory->signal_bind().connect(sigc::mem_fun(*this, &invoice_page::bind_amount));
        factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::teardown)));
        column->set_expand(false);
        column->set_expand(false);
        view->append_column(column);
}

void gui::invoice_page::setup(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
        if (!list_item)
        {
                syslog(LOG_CRIT, "The list_item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        list_item->set_child(*Gtk::make_managed<Gtk::Entry>());
}

void gui::invoice_page::teardown(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
        if (!list_item)
        {
                syslog(LOG_CRIT, "The list_item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        list_item->unset_child();
}

void gui::invoice_page::bind_quantity(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
        if (!list_item)
        {
                syslog(LOG_CRIT, "The list_item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        auto columns = std::dynamic_pointer_cast<column_entries>(list_item->get_item());
        if (!columns)
        {
                syslog(LOG_CRIT, "The bind quantity columns are not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        auto entry = dynamic_cast<Gtk::Entry*>(list_item->get_child());
        if (!entry)
        {
                syslog(LOG_CRIT, "The bind quantity entry is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        entry->set_max_length(limit::MAX_QUANTITY);
        entry->set_text(std::to_string(columns->quantity));
        entry->signal_changed().connect([entry, columns, this] () {
                std::string text{entry->get_text()};
                if (text.empty())
                {
                        text = "0";
                }

                std::regex int_regex(R"(^[0-9]+$)");
                bool correct_format{std::regex_search(text, int_regex)};
                if (correct_format)
                {
                        columns->quantity = std::stoi(text);
                }
                else
                {
                        this->wrong_data_in_quantity_column->show();
                        entry->select_region(0, limit::MAX_QUANTITY);
                        columns->quantity = 0;
                }
        });
}

void gui::invoice_page::bind_description(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
        if (!list_item)
        {
                syslog(LOG_CRIT, "The list_item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        auto columns = std::dynamic_pointer_cast<column_entries>(list_item->get_item());
        if (!columns)
        {
                syslog(LOG_CRIT, "The bind description columns are not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        auto entry = dynamic_cast<Gtk::Entry*>(list_item->get_child());
        if (!entry)
        {
                syslog(LOG_CRIT, "The bind description entry is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        entry->set_max_length(limit::MAX_DESCRIPTION);
        entry->set_text(columns->description);
        entry->signal_changed().connect([entry, columns, this] () {
                columns->description = entry->get_text();
        });
}

void gui::invoice_page::bind_amount(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
        if (!list_item)
        {
                syslog(LOG_CRIT, "The list_item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        auto columns = std::dynamic_pointer_cast<column_entries>(list_item->get_item());
        if (!columns)
        {
                syslog(LOG_CRIT, "The bind amount columns are not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        auto entry = dynamic_cast<Gtk::Entry*>(list_item->get_child());
        if (!entry)
        {
                syslog(LOG_CRIT, "The bind amount entry is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        std::ostringstream amount_precision{""};
        amount_precision << std::fixed << std::setprecision(2) << columns->amount;
        entry->set_max_length(limit::MAX_AMOUNT);
        entry->set_text(amount_precision.str());
        entry->signal_changed().connect([entry, columns, this] () {
                std::string text{entry->get_text()};
                if (text.empty())
                        text = "0";

                std::regex double_regex(R"(^-?([0-9]+(\.[0-9]*)?|\.[0-9]+)$)");
                bool correct_format{std::regex_search(text, double_regex)};
                if (correct_format)
                {
                        columns->amount = std::stod(text);
                        auto ancestor{entry->get_ancestor(Gtk::ColumnView::get_type())};
                        if (ancestor->get_name() == "description_view")
                        {
                                std::ostringstream doss{""};
                                doss << std::fixed << std::setprecision(2) << compute_total(this->description_store);
                                this->description_total_label->set_text("Total: R " + doss.str());
                                this->description_total = doss.str();
                        }

                        if (ancestor->get_name() == "material_view")
                        {
                                std::ostringstream moss{""};
                                moss << std::fixed << std::setprecision(2) << compute_total(this->material_store);
                                this->material_total_label->set_text("Total: R " + moss.str());
                                this->material_total = moss.str();
                        }

                        std::ostringstream goss{""};
                        goss << std::fixed << std::setprecision(2) << this->compute_grand_total();
                        this->grand_total_label->set_text("Grand Total: R " + goss.str());
                        this->grand_total = goss.str();
                }
                else
                {
                        this->wrong_data_in_amount_column->show();
                        entry->select_region(0, limit::MAX_AMOUNT);
                        columns->amount = 0;
                }
        });
}

double gui::invoice_page::compute_total(const Glib::RefPtr<Gio::ListStore<column_entries>>& store)
{
        double total{0.0};
        if (!store)
        {
                syslog(LOG_CRIT, "The store is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
                for (guint i = 0; i < store->get_n_items(); ++i)
                {
                        auto item = store->get_item(i);
                        if (item)
                        {
                                total += item->amount;
                        }
                }
        }

        return total;
}

double gui::invoice_page::compute_grand_total()
{
        double total{this->compute_total(this->description_store) + this->compute_total(this->material_store)};

        return total;
}

data::invoice gui::invoice_page::extract_invoice_data()
{
        data::invoice data{};
        data.set_business_name(this->search_entry->get_text());
        data.set_invoice_number(this->invoice_number->get_text());
        data.set_invoice_date(this->invoice_date->get_text());
        data.set_job_card_number(this->job_card->get_text());
        data.set_order_number(this->order_number->get_text());
        data.set_description_total(this->description_total);
        data.set_material_total(this->material_total);
        data.set_grand_total(this->grand_total);
        std::vector<data::column> description_columns{this->retrieve_column_data(this->description_store)};
        data.set_description_column(description_columns);
        std::vector<data::column> material_columns{this->retrieve_column_data(this->material_store)};
        data.set_material_column(material_columns);

        return data;
}

std::vector<data::column> gui::invoice_page::retrieve_column_data(const Glib::RefPtr<Gio::ListStore<column_entries>>& store)
{
        std::vector<data::column> columns{};
        if (!store)
        {
                syslog(LOG_CRIT, "The store is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
                for (guint i = 0; i < store->get_n_items(); ++i)
                {
                        auto item = store->get_item(i);
                        if (item)
                        {
                                data::column column;
                                column.set_quantity(item->quantity);
                                column.set_description(item->description);
                                column.set_amount(item->amount);
                                columns.push_back(column);
                        }
                }
        }

        return columns;
}

void gui::invoice_page::send_email(const std::vector<data::invoice>& _data)
{
        if (_data.empty())
        {
                syslog(LOG_CRIT, "The invoice data is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        std::thread([this, _data] () {
                bool success = client_invoice.send_email(_data);
                this->email_success = success;
                this->email_dispatcher.emit();
        }).detach();
}

void gui::invoice_page::on_draw_page(const Glib::RefPtr<Gtk::PrintContext>& _context, int _page_nr)
{
        for (const auto& range : this->page_ranges)
        {
                if (_page_nr >= range.start_page && _page_nr < range.start_page + range.page_count)
                {
                        std::shared_ptr<poppler::document> document{this->documents_to_print[range.document_index]};
                        if (!document)
                                return;

                        int local_page = _page_nr - range.start_page;

                        auto page = document->create_page(local_page);
                        if (!page)
                                return;

                        poppler::page_renderer renderer;
                        renderer.set_render_hint(poppler::page_renderer::antialiasing, true);
                        renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true);

                        double dpi_x = 190;
                        double dpi_y = 290;
                        const double target_dpi = std::max(dpi_x, dpi_y);

                        auto image = renderer.render_page(page, target_dpi, target_dpi);
                        if (!image.is_valid())
                                return;

                        auto cairo_surface = Cairo::ImageSurface::create(
                                        reinterpret_cast<unsigned char*> (image.data()), Cairo::Surface::Format::ARGB32,
                                        image.width(), image.height(),
                                        image.bytes_per_row());

                        double sx = _context->get_width() / image.width();
                        double sy = _context->get_height() / image.height();
                        double scale_factor = std::min(sx, sy);
                        auto cr = _context->get_cairo_context();
                        cr->save();
                        cr->scale(scale_factor, scale_factor);
                        cr->set_source(cairo_surface, 0, 0);
                        cr->paint();
                        cr->restore();
                        return;
                }
        }
}

void gui::invoice_page::setup_page()
{
        this->page_setup = Gtk::PageSetup::create();
        if (!this->page_setup)
        {
                syslog(LOG_CRIT, "The page_setup is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->page_setup->set_orientation(Gtk::PageOrientation::PORTRAIT);
        this->page_setup->set_paper_size(Gtk::PaperSize(Gtk::PAPER_NAME_A4));
        this->page_setup->set_top_margin(0, Gtk::Unit::POINTS);
        this->page_setup->set_bottom_margin(0, Gtk::Unit::POINTS);
        this->page_setup->set_left_margin(0, Gtk::Unit::POINTS);
        this->page_setup->set_right_margin(0, Gtk::Unit::POINTS);
}

void gui::invoice_page::compute_number_of_pages(const std::vector<data::invoice>& _data)
{
        if (_data.empty())
        {
                syslog(LOG_CRIT, "The invoice data is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        std::size_t index{0};
        this->number_of_pages = 0;
        this->page_ranges.clear();
        this->documents_to_print.clear();
        std::vector<data::pdf_invoice> invoices_to_print{client_invoice.create_pdf_to_print(_data)};
        for (const data::pdf_invoice& invoice_to_print : invoices_to_print)
        {
                std::shared_ptr<poppler::document> doc{this->pdf.generate_for_print(invoice_to_print)};
                if (!doc)
                {
                        continue;
                }

                int page_count = doc->pages();
                this->documents_to_print.push_back(doc);

                this->page_ranges.push_back(page_range{this->number_of_pages, page_count, index});
                this->number_of_pages += page_count;
                ++index;
        }
}

void gui::invoice_page::print_invoice(const std::vector<data::invoice>& _data)
{
        if (_data.empty())
        {
                syslog(LOG_CRIT, "The invoice data is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        Glib::RefPtr<Gtk::PrintOperation> print_operation{Gtk::PrintOperation::create()};
        if (!print_operation)
        {
                syslog(LOG_CRIT, "The print_operation is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        print_operation->set_default_page_setup(this->page_setup);
        print_operation->set_unit(Gtk::Unit::POINTS);
        print_operation->set_has_selection(false);
        print_operation->set_job_name("Invoice");
        print_operation->set_use_full_page(true);
        print_operation->set_show_progress(true);
        print_operation->set_allow_async(true);
        print_operation->signal_draw_page().connect(
                sigc::mem_fun(*this, &invoice_page::on_draw_page));
        print_operation->signal_done().connect(sigc::bind(
                sigc::mem_fun(*this, &invoice_page::on_printoperation_done), print_operation));
        compute_number_of_pages(_data);
        std::thread([&] () {
                if (this->number_of_pages > 0)
                {
                        print_operation->set_n_pages(this->number_of_pages);
                        print_operation->run(Gtk::PrintOperation::Action::PREVIEW);
                }
                this->print_dispatcher.emit();
        }).detach();
}

void gui::invoice_page::on_printoperation_done(Gtk::PrintOperation::Result _result, const Glib::RefPtr<Gtk::PrintOperation>& _op)
{
        if (_result == Gtk::PrintOperation::Result::ERROR)
        {
                syslog(LOG_CRIT, "Failed to complete the print operation - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else if (_result == Gtk::PrintOperation::Result::CANCEL)
        {
                syslog(LOG_CRIT, "The print operation was canceled - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else if (_result == Gtk::PrintOperation::Result::IN_PROGRESS)
        {
                syslog(LOG_CRIT, "The print operation is in progress - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }

        std::string has_printer{_op->get_print_settings()->get_printer()};
        if (has_printer.empty())
        {
                syslog(LOG_CRIT, "No printer is connected - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
}

void gui::invoice_page::populate(const std::string& _business_name)
{
        this->invoice_store->remove_all();
        if (_business_name.empty())
        {
                this->email_button->set_sensitive(false);
                this->print_button->set_sensitive(false);
        }
        else
        {
                std::vector<data::invoice> db_invoices{client_invoice.search(_business_name, this->db)};
                populate_list_store(db_invoices);
        }
}

void gui::invoice_page::add(const data::invoice& _invoice)
{
        if (!_invoice.is_valid())
        {
                syslog(LOG_CRIT, "The invoice data is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->invoice_store->append(invoice_entries::create(_invoice));
        Glib::signal_timeout().connect_once([this]() {
                this->invoices_adjustment->set_value(this->invoices_adjustment->get_upper());
        }, 30);
}

void gui::invoice_page::connect_email_alert()
{
        if (!this->email_confirmation)
        {
                syslog(LOG_CRIT, "The email_confirmation is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->email_confirmation->signal_response().connect([this] (int response) {
                std::vector<data::invoice> data{this->invoices_selected};
                if (response == GTK_RESPONSE_YES)
                {
                        syslog(LOG_INFO, "User chose to email the information - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
                        this->email_confirmation->hide();
                        this->send_email(this->invoices_selected);
                }
                else
                {
                        syslog(LOG_INFO, "User chose not to email the information - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
                        this->email_confirmation->hide();
                }
        });
}

void gui::invoice_page::connect_email_button()
{
        if (!this->email_button)
        {
                syslog(LOG_CRIT, "The email_button is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->email_button->set_sensitive(false);
        this->email_button->signal_clicked().connect([this] () {
                this->email_confirmation->show();
        });
}

void gui::invoice_page::connect_print_alert()
{
        if (!this->print_confirmation)
        {
                syslog(LOG_CRIT, "The print_confirmation is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->print_confirmation->signal_response().connect([this] (int response) {
                if (response == GTK_RESPONSE_YES)
                {
                        syslog(LOG_INFO, "User chose to print the information - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
                        this->print_confirmation->hide();
                        print_invoice(this->invoices_selected);
                }
                else
                {
                        syslog(LOG_INFO, "User chose not to print the information - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
                        this->print_confirmation->hide();
                }
        });
}

void gui::invoice_page::connect_invoice_view()
{
        this->invoice_store = std::shared_ptr<Gio::ListStore<invoice_entries>>{
                Gio::ListStore<invoice_entries>::create()};
        if (!this->invoice_store)
        {
                syslog(LOG_CRIT, "The invoice_store is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        if (!this->invoice_view)
        {
                syslog(LOG_CRIT, "The invoice_view is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        Glib::RefPtr<Gtk::MultiSelection> selection_model = Gtk::MultiSelection::create(this->invoice_store);
        if (!selection_model)
        {
                syslog(LOG_CRIT, "The invoice selection_model is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->invoice_view->set_name("invoice_view");
        this->invoice_view->set_model(selection_model);
        this->invoice_view->set_single_click_activate(false);
        this->invoice_view->signal_activate().connect(
                sigc::mem_fun(*this, &invoice_page::edit_known_invoice));

        selection_model->signal_selection_changed().connect(
                        sigc::mem_fun(*this, &invoice_page::selected_invoice));

        invoices(this->invoice_view);
}

void gui::invoice_page::connect_no_internet_alert()
{
        if (!this->email_no_internet)
        {
                syslog(LOG_CRIT, "The email_no_internet is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->email_no_internet->signal_response().connect([this] (int response) {
                switch(response)
                {
                        case GTK_RESPONSE_CLOSE:
                                this->email_no_internet->hide();
                                break;
                        default:
                                this->email_no_internet->hide();
                                break;
                }
        });
}

void gui::invoice_page::connect_print_button()
{
        if (!this->print_button)
        {
                syslog(LOG_CRIT, "The print_button is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->print_button->set_sensitive(false);
        this->print_button->signal_clicked().connect([this] () {
                this->print_confirmation->show();
        });
}

void gui::invoice_page::connect_no_printer_alert()
{
        if (!this->print_no_printer)
        {
                syslog(LOG_CRIT, "The print_no_printer is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        this->print_no_printer->signal_response().connect([this] (int response) {
                switch(response)
                {
                        case GTK_RESPONSE_CLOSE:
                                this->print_no_printer->hide();
                                break;
                        default:
                                this->print_no_printer->hide();
                                break;
                }
        });
}

void gui::invoice_page::invoices(const std::unique_ptr<Gtk::ListView>& view)
{
        if (!view)
        {
                syslog(LOG_CRIT, "The view is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        Glib::RefPtr<Gtk::SignalListItemFactory> factory = Gtk::SignalListItemFactory::create();
        if (!factory)
        {
                syslog(LOG_CRIT, "The factory is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::invoice_setup)));
        factory->signal_bind().connect(sigc::mem_fun(*this, &invoice_page::bind_invoices));
        factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::invoice_teardown)));
        view->set_factory(factory);
}

void gui::invoice_page::invoice_setup(const Glib::RefPtr<Gtk::ListItem>& _item)
{
        if (!_item)
        {
                syslog(LOG_CRIT, "The _item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        auto label = Gtk::make_managed<Gtk::Label>("", Gtk::Align::START);
        if (!label)
        {
                syslog(LOG_CRIT, "The label is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        label->set_hexpand(true);
        label->set_vexpand(false);
        label->set_size_request(200, 35);
        _item->set_child(*label);
}

void gui::invoice_page::invoice_teardown(const Glib::RefPtr<Gtk::ListItem>& _item)
{
        if (!_item)
        {
                syslog(LOG_CRIT, "The _item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        _item->unset_child();
}

void gui::invoice_page::bind_invoices(const Glib::RefPtr<Gtk::ListItem>& _item)
{
        if (!_item)
        {
                syslog(LOG_CRIT, "The _item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        auto data = std::dynamic_pointer_cast<invoice_entries>(_item->get_item());
        if (!data)
        {
                syslog(LOG_CRIT, "The data is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        auto label = dynamic_cast<Gtk::Label*>(_item->get_child());
        if (!label)
        {
                syslog(LOG_CRIT, "The label is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        data::invoice invoice{data->invoice};
        std::string details{"# " + invoice.get_invoice_number() + ", " + invoice.get_business_name() + ", " + invoice.get_invoice_date() + " "};
        label->set_text(details);
}

void gui::invoice_page::populate_list_store(const std::vector<data::invoice>& _invoices)
{
        if (_invoices.empty())
        {
                syslog(LOG_CRIT, "The _invoices are not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        for (const auto& invoice : _invoices)
        {
                if (invoice.is_valid())
                {
                        this->invoice_store->append(invoice_entries::create(invoice));
                        Glib::signal_timeout().connect_once([this]() {
                                this->invoices_adjustment->set_value(this->invoices_adjustment->get_upper());
                        }, 30);
                }
        }
}

void gui::invoice_page::selected_invoice(uint _position, uint _items_selected)
{
        ++_position;
        ++_items_selected;

        this->invoices_selected.clear();
        Glib::RefPtr<Gtk::SelectionModel> selection_model = this->invoice_view->get_model();
        if (!selection_model)
        {
                syslog(LOG_CRIT, "The selection_model is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        Glib::RefPtr<const Gtk::Bitset> items_list{selection_model->get_selection()};
        for (guint item : *items_list)
        {
                auto data = this->invoice_store->get_item(item);
                if (!data)
                        return;
                auto invoice = std::dynamic_pointer_cast<invoice_entries>(data);
                if (!invoice)
                        return;

                data::invoice temp{invoice->invoice};
                this->invoices_selected.push_back(invoice->invoice);
        }
        this->email_button->set_sensitive(true);
        this->print_button->set_sensitive(true);
}

void gui::invoice_page::email_sent()
{
        if (this->email_success == false)
        {
                this->email_no_internet->show();
        }
        else
        {
                syslog(LOG_INFO, "User emailed an invoice - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
}

void gui::invoice_page::printed()
{
        if (this->print_success == false)
        {
                this->print_no_printer->show();
        }
        else
        {
                syslog(LOG_INFO, "User printed an invoice - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
}

void gui::invoice_page::populate_description_store(const data::invoice& _invoice)
{
        if (!_invoice.is_valid())
        {
                syslog(LOG_CRIT, "The invoice data is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        for (const data::column& column : _invoice.get_description_column())
        {
                if (column.is_valid())
                {
                        this->description_store->append(column_entries::create(
                                                column.get_quantity(),
                                                column.get_description(),
                                                column.get_amount()));
                        Glib::signal_timeout().connect_once([this]() {
                                this->description_adjustment->set_value(this->description_adjustment->get_upper());
                        }, 30);
                }
        }
}

void gui::invoice_page::populate_material_store(const data::invoice& _invoice)
{
        if (!_invoice.is_valid())
        {
                syslog(LOG_CRIT, "The invoice data is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        for (const data::column& column : _invoice.get_description_column())
        {
                if (column.is_valid())
                {
                        this->material_store->append(column_entries::create(
                                                column.get_quantity(),
                                                column.get_description(),
                                                column.get_amount()));
                        Glib::signal_timeout().connect_once([this]() {
                                this->material_adjustment->set_value(this->material_adjustment->get_upper());
                        }, 30);
                }
        }
}
