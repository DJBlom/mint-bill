/***********************************************************
 * Contents: Invoice Page class implementation
 * Author: Dawid J. Blom
 * Date: November 28, 2024
 *
 * NOTE:
 **********************************************************/
#include <invoice_page.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <regex>


namespace limit {
        constexpr std::uint8_t MAX_QUANTITY{9};
        constexpr std::uint8_t MAX_AMOUNT{15};
        constexpr std::uint16_t MAX_DESCRIPTION{500};
}

gui::invoice_page::~invoice_page()
{

}

bool gui::invoice_page::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        bool created{false};
        if (_ui_builder)
        {
                created = true;
                create_views(_ui_builder);
                create_entries(_ui_builder);
                create_dialogs(_ui_builder);
                create_buttons(_ui_builder);
                connect_search();
                connect_save_button();
                connect_email_button();
                connect_material_view();
                connect_description_view();
                connect_save_alert();
                connect_email_alert();
                connect_no_internet_alert();
                connect_wrong_info_alert();
                connect_wrong_data_in_amount_column_alert();
                connect_wrong_data_in_quantity_column_alert();
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
        this->email_confirmation = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-email-alert")};
        this->email_no_internet = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-email-no-internet-alert")};
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
        this->email_button = std::unique_ptr<Gtk::Button>{
                _ui_builder->get_widget<Gtk::Button>("known-invoice-email-button")};
}

void gui::invoice_page::connect_search()
{
        if (this->search_entry)
        {
                search_entry->signal_search_changed().connect([this] () {
                        this->invoice_number->set_text("1");
                        this->invoice_date->set_text("10-12-2024");
                });
        }
}

void gui::invoice_page::connect_save_button()
{
        if (save_button)
        {
                save_button->signal_clicked().connect([this] () {
                        this->save_alert_dialog->show();
                });
        }
}

void gui::invoice_page::connect_email_button()
{
        if (this->email_button)
        {
                this->email_button->set_sensitive(false);
                this->email_button->signal_clicked().connect([this] () {
                        this->email_confirmation->show();
                });
        }
}

void gui::invoice_page::connect_description_view()
{
        this->description_store = std::shared_ptr<Gio::ListStore<column_entries>>{
                Gio::ListStore<column_entries>::create()};
        Glib::RefPtr<Gtk::MultiSelection> selection_model = Gtk::MultiSelection::create(this->description_store);
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
        if (description_add_button)
        {
                description_add_button->signal_clicked().connect([this] () {
                        this->description_store->append(column_entries::create());
                        Glib::signal_timeout().connect_once([this]() {
                                this->description_adjustment->set_value(this->description_adjustment->get_upper());
                        }, 30);
                });
        }
}

void gui::invoice_page::connect_description_delete_button(const Glib::RefPtr<Gtk::MultiSelection>& selection_model)
{
        if (description_delete_button)
        {
                description_delete_button->signal_clicked().connect([selection_model, this] () {
                        Glib::RefPtr<const Gtk::Bitset> indices = selection_model->get_selection();
                        while (indices->get_size() != 0)
                        {
                                this->description_store->remove(indices->get_minimum());
                        }
                });
        }
}

void gui::invoice_page::connect_description_list_store()
{
        this->description_store->signal_items_changed().connect(
                        sigc::mem_fun(*this, &invoice_page::update_description_total));
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
        Glib::RefPtr<Gtk::MultiSelection> selection_model = Gtk::MultiSelection::create(this->material_store);
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
        if (material_add_button)
        {
                material_add_button->signal_clicked().connect([this] () {
                        this->material_store->append(column_entries::create());
                        Glib::signal_timeout().connect_once([this]() {
                                this->material_adjustment->set_value(this->material_adjustment->get_upper());
                        }, 30);
                });
        }
}

void gui::invoice_page::connect_material_delete_button(const Glib::RefPtr<Gtk::MultiSelection>& selection_model)
{
        if (material_delete_button)
        {
                material_delete_button->signal_clicked().connect([selection_model, this] () {
                        Glib::RefPtr<const Gtk::Bitset> indices = selection_model->get_selection();
                        while (indices->get_size() != 0)
                        {
                                this->material_store->remove(indices->get_minimum());
                        }
                });
        }
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
        this->save_alert_dialog->signal_response().connect([this] (int response) {
                data::invoice data{extract_invoice_data()};
                switch(response)
                {
                        case GTK_RESPONSE_YES:
                                if (this->client_invoice.save(data, this->db) == false)
                                {
                                        this->save_alert_dialog->hide();
                                        this->wrong_info_alert_dialog->show();
                                }
                                else
                                {
                                        this->save_alert_dialog->hide();
                                        this->description_store->remove_all();
                                        this->material_store->remove_all();
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

void gui::invoice_page::connect_email_alert()
{
        this->email_confirmation->signal_response().connect([this] (int response) {
                data::invoice data{extract_invoice_data()};
                switch(response)
                {
                        case GTK_RESPONSE_YES:
                                if (this->client_invoice.send_email(data) == false)
                                {
                                        this->email_confirmation->hide();
                                        this->email_no_internet->show();
                                }
                                else
                                {
                                        this->email_confirmation->hide();
                                }
                                break;
                        case GTK_RESPONSE_NO:
                                this->email_confirmation->hide();
                                break;
                        default:
                                this->email_confirmation->hide();
                                break;
                }
        });
}

void gui::invoice_page::connect_no_internet_alert()
{
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

void gui::invoice_page::connect_wrong_info_alert()
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
}

void gui::invoice_page::connect_wrong_data_in_amount_column_alert()
{
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
        Glib::RefPtr<Gtk::SignalListItemFactory> factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::setup)));
        factory->signal_bind().connect(sigc::mem_fun(*this, &invoice_page::bind_quantity));
        factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::teardown)));
        Glib::RefPtr<Gtk::ColumnViewColumn> column = Gtk::ColumnViewColumn::create("Quantity", factory);
        column->set_expand(false);
        view->append_column(column);
}

void gui::invoice_page::description_column(const std::unique_ptr<Gtk::ColumnView>& view)
{
        Glib::RefPtr<Gtk::SignalListItemFactory> factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::setup)));
        factory->signal_bind().connect(sigc::mem_fun(*this, &invoice_page::bind_description));
        factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::teardown)));
        Glib::RefPtr<Gtk::ColumnViewColumn> column = Gtk::ColumnViewColumn::create("Description", factory);
        column->set_expand(true);
        view->append_column(column);
}

void gui::invoice_page::amount_column(const std::unique_ptr<Gtk::ColumnView>& view)
{
        Glib::RefPtr<Gtk::SignalListItemFactory> factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::setup)));
        factory->signal_bind().connect(sigc::mem_fun(*this, &invoice_page::bind_amount));
        factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::teardown)));
        Glib::RefPtr<Gtk::ColumnViewColumn> column = Gtk::ColumnViewColumn::create("Amount", factory);
        column->set_expand(false);
        view->append_column(column);
}

void gui::invoice_page::setup(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
        list_item->set_child(*Gtk::make_managed<Gtk::Entry>());
}

void gui::invoice_page::teardown(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
        list_item->unset_child();
}

void gui::invoice_page::bind_quantity(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
        auto columns = std::dynamic_pointer_cast<column_entries>(list_item->get_item());
        if (!columns)
                return;

        auto entry = dynamic_cast<Gtk::Entry*>(list_item->get_child());
        if (!entry)
                return;

        entry->set_max_length(limit::MAX_QUANTITY);
        entry->signal_changed().connect([entry, columns, this] () {
                std::string text{entry->get_text()};
                if (text.empty())
                        text = "0";

                std::regex int_regex(R"(^[0-9]+$)");
                bool correct_in_format{std::regex_search(text, int_regex)};
                if (correct_in_format)
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
        auto columns = std::dynamic_pointer_cast<column_entries>(list_item->get_item());
        if (!columns)
                return;

        auto entry = dynamic_cast<Gtk::Entry*>(list_item->get_child());
        if (!entry)
                return;

        entry->set_max_length(limit::MAX_DESCRIPTION);
        entry->signal_changed().connect([entry, columns, this] () {
               columns->description = entry->get_text();
        });
}

void gui::invoice_page::bind_amount(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
        auto columns = std::dynamic_pointer_cast<column_entries>(list_item->get_item());
        if (!columns)
                return;

        auto entry = dynamic_cast<Gtk::Entry*>(list_item->get_child());
        if (!entry)
                return;

        entry->set_max_length(limit::MAX_AMOUNT);
        entry->signal_changed().connect([entry, columns, this] () {
                std::string text{entry->get_text()};
                if (text.empty())
                        text = "0";

                std::regex double_regex(R"(^-?([0-9]+(\.[0-9]*)?|\.[0-9]+)$)");
                bool correct_double_format{std::regex_search(text, double_regex)};
                if (correct_double_format)
                {
                        columns->amount = std::stod(text);
                        auto ancestor{entry->get_ancestor(Gtk::ColumnView::get_type())};
                        if (ancestor->get_name() == "description_view")
                        {
                                std::ostringstream doss{""};
                                doss << std::fixed << std::setprecision(2) << compute_total(this->description_store);
                                this->description_total_label->set_text("Total: R " + doss.str());
                        }
                        else
                        {
                                std::ostringstream moss{""};
                                moss << std::fixed << std::setprecision(2) << compute_total(this->material_store);
                                this->material_total_label->set_text("Total: R " + moss.str());
                        }

                        std::ostringstream goss{""};
                        goss << std::fixed << std::setprecision(2) << this->compute_grand_total();
                        this->grand_total_label->set_text("Grand Total: R " + goss.str());
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
        for (guint i = 0; i < store->get_n_items(); ++i)
        {
                auto item = store->get_item(i);
                if (item)
                {
                        total += item->amount;
                }
        }

        return total;
}

double gui::invoice_page::compute_grand_total()
{
        double total{0.0};
        total = (this->compute_total(this->description_store) + this->compute_total(this->material_store));

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

        return columns;
}
