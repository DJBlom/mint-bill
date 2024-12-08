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
}

namespace

gui::invoice_page::~invoice_page()
{

}

bool gui::invoice_page::create(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool created{false};
        if (verify_ui_builder(ui_builder) == true)
        {
                created = true;
                connect_search(ui_builder);
                connect_save_button(ui_builder);
                connect_save_alert(ui_builder);
                connect_wrong_info_alert(ui_builder);
                connect_wrong_data_in_quantity_column_alert(ui_builder);
                connect_wrong_data_in_amount_column_alert(ui_builder);
                connect_grand_total_label(ui_builder);
                connect_description_view(ui_builder);
                connect_material_view(ui_builder);
        }

        return created;
}

bool gui::invoice_page::verify_ui_builder(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool verified{false};
        if (ui_builder)
        {
                verified = true;
        }

        return verified;
}

void gui::invoice_page::connect_search(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::shared_ptr<Gtk::SearchEntry> search_entry{ui_builder->get_widget<Gtk::SearchEntry>
                ("invoice-search-entry")};
        if (search_entry)
        {
                search_entry->signal_search_changed().connect([search_entry] () {
                        std::cout << "Searching: " << search_entry->get_text() << std::endl;
                });
        }
}

void gui::invoice_page::connect_save_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::Button> save_button{ui_builder->get_widget<Gtk::Button>
                ("new-invoice-save-button")};
        if (save_button)
        {
                save_button->signal_clicked().connect([this] () {
                        this->save_alert_dialog->show();
                });
        }
}

void gui::invoice_page::connect_save_alert(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->save_alert_dialog = std::unique_ptr<Gtk::MessageDialog>
                (ui_builder->get_widget<Gtk::MessageDialog>("invoice-save-button-alert"));
        this->save_alert_dialog->signal_response().connect([this] (int response) {
                switch(response)
                {
                        case GTK_RESPONSE_YES:
                                // Todo (Handle the saving of the data)
                                this->save_alert_dialog->hide();
                                this->wrong_info_alert_dialog->show();
                                this->description_store->remove_all();
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

void gui::invoice_page::connect_wrong_info_alert(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->wrong_info_alert_dialog = std::unique_ptr<Gtk::MessageDialog>
                (ui_builder->get_widget<Gtk::MessageDialog>("invoice-wrong-info-alert"));
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

void gui::invoice_page::connect_wrong_data_in_quantity_column_alert(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->wrong_data_in_quantity_column = std::unique_ptr<Gtk::MessageDialog>
                (ui_builder->get_widget<Gtk::MessageDialog>("invoice-data-in-quantity-column-alert"));
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

void gui::invoice_page::connect_wrong_data_in_amount_column_alert(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->wrong_data_in_amount_column = std::unique_ptr<Gtk::MessageDialog>
                (ui_builder->get_widget<Gtk::MessageDialog>("invoice-data-in-amount-column-alert"));
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

void gui::invoice_page::connect_grand_total_label(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->grand_total = std::unique_ptr<Gtk::Label>
                (ui_builder->get_widget<Gtk::Label>("new-invoice-total-label"));
}

void gui::invoice_page::connect_description_view(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::shared_ptr<Gtk::ColumnView> view{ui_builder->get_widget<Gtk::ColumnView>
                ("new-invoice-description")};
        this->description_store = Gio::ListStore<column_entries>::create();
        Glib::RefPtr<Gtk::MultiSelection> selection_model = Gtk::MultiSelection::create(this->description_store);
        view->set_model(selection_model);
        view->set_name("description_view");

        connect_description_add_button(ui_builder);
        connect_description_delete_button(selection_model, ui_builder);
        connect_description_list_store(ui_builder);

        quantity_column(view);
        description_column(view);
        amount_column(view);
}

void gui::invoice_page::connect_description_add_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::Button> description_add_button{ui_builder->get_widget<Gtk::Button>
                ("invoice-description-add-button")};
        Glib::RefPtr<Gtk::Adjustment> adjustment{ui_builder->get_object<Gtk::Adjustment>
                ("new-invoice-description-column-view-vadjustment")};
        if (description_add_button)
        {
                description_add_button->signal_clicked().connect([adjustment, this] () {
                        this->description_store->append(column_entries::create());
                        Glib::signal_timeout().connect_once([adjustment]() {
                                adjustment->set_value(adjustment->get_upper());
                        }, 30);
                });
        }
}

void gui::invoice_page::connect_description_delete_button(const Glib::RefPtr<Gtk::MultiSelection>& selection_model,
                                                          const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::Button> description_delete_button{ui_builder->get_widget<Gtk::Button>
                ("invoice-description-delete-button")};
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

void gui::invoice_page::connect_description_list_store(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->description_total = std::unique_ptr<Gtk::Label>(
                        ui_builder->get_widget<Gtk::Label>("new-invoice-description-total-label"));
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
        this->description_total->set_text("Total: R " + doss.str());

        std::ostringstream moss{""};
        moss << std::fixed << std::setprecision(2) << compute_total(this->material_store);
        this->material_total->set_text("Total: R " + moss.str());

        std::ostringstream goss{""};
        goss << std::fixed << std::setprecision(2) << this->compute_grand_total();
        this->grand_total->set_text("Grand Total: R " + goss.str());
}

void gui::invoice_page::connect_material_view(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::shared_ptr<Gtk::ColumnView> view{ui_builder->get_widget<Gtk::ColumnView>
                ("new-invoice-material")};
        this->material_store = Gio::ListStore<column_entries>::create();
        Glib::RefPtr<Gtk::MultiSelection> selection_model = Gtk::MultiSelection::create(this->material_store);
        view->set_model(selection_model);
        view->set_name("material_view");

        connect_material_add_button(ui_builder);
        connect_material_delete_button(selection_model, ui_builder);
        connect_material_list_store(ui_builder);

        quantity_column(view);
        description_column(view);
        amount_column(view);
}

void gui::invoice_page::connect_material_add_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::Button> material_add_button{ui_builder->get_widget<Gtk::Button>
                ("invoice-material-button")};
        Glib::RefPtr<Gtk::Adjustment> adjustment{ui_builder->get_object<Gtk::Adjustment>
                ("new-invoice-material-column-view-adjustment")};
        if (material_add_button)
        {
                material_add_button->signal_clicked().connect([adjustment, this] () {
                        this->material_store->append(column_entries::create());
                        Glib::signal_timeout().connect_once([adjustment]() {
                                adjustment->set_value(adjustment->get_upper());
                        }, 30);
                });
        }
}

void gui::invoice_page::connect_material_delete_button(const Glib::RefPtr<Gtk::MultiSelection>& selection_model,
                                                          const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::Button> material_delete_button{ui_builder->get_widget<Gtk::Button>
                ("invoice-material-delete-button")};
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

void gui::invoice_page::connect_material_list_store(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->material_total = std::unique_ptr<Gtk::Label>(
                        ui_builder->get_widget<Gtk::Label>("new-invoice-material-total-label"));
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
        this->description_total->set_text("Total: R " + doss.str());

        std::ostringstream moss{""};
        moss << std::fixed << std::setprecision(2) << compute_total(this->material_store);
        this->material_total->set_text("Total: R " + moss.str());

        std::ostringstream goss{""};
        goss << std::fixed << std::setprecision(2) << this->compute_grand_total();
        this->grand_total->set_text("Grand Total: R " + goss.str());
}

void gui::invoice_page::quantity_column(const std::shared_ptr<Gtk::ColumnView>& view)
{
        Glib::RefPtr<Gtk::SignalListItemFactory> factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::setup)));
        factory->signal_bind().connect(sigc::mem_fun(*this, &invoice_page::bind_quantity));
        factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::teardown)));
        Glib::RefPtr<Gtk::ColumnViewColumn> column = Gtk::ColumnViewColumn::create("Quantity", factory);
        column->set_expand(false);
        view->append_column(column);
}

void gui::invoice_page::description_column(const std::shared_ptr<Gtk::ColumnView>& view)
{
        Glib::RefPtr<Gtk::SignalListItemFactory> factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::setup)));
        factory->signal_bind().connect(sigc::mem_fun(*this, &invoice_page::bind_description));
        factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &invoice_page::teardown)));
        Glib::RefPtr<Gtk::ColumnViewColumn> column = Gtk::ColumnViewColumn::create("Description", factory);
        column->set_expand(true);
        view->append_column(column);
}

void gui::invoice_page::amount_column(const std::shared_ptr<Gtk::ColumnView>& view)
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
                if (all_of(text.begin(), text.end(), ::isdigit) == true)
                        columns->quantity = std::stoi(text);
                else
                {
                        this->wrong_data_in_quantity_column->show();
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

        entry->signal_changed().connect([entry, columns, this] () {
               columns->description = entry->get_text();
               std::cout << "Data: " << columns->amount << std::endl;
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
                std::regex double_regex(R"(^-?([0-9]+(\.[0-9]*)?|\.[0-9]+)$)");
                bool correct_double_format{std::regex_search(text, double_regex)};
                if (correct_double_format)
                {
                        columns->amount = std::stod(text);
                        std::unique_ptr<Gtk::Widget> ancestor{entry->get_ancestor(Gtk::ColumnView::get_type())};
                        if (ancestor->get_name() == "description_view")
                        {
                                std::ostringstream doss{""};
                                doss << std::fixed << std::setprecision(2) << compute_total(this->description_store);
                                this->description_total->set_text("Total: R " + doss.str());
                        }
                        else
                        {
                                std::ostringstream moss{""};
                                moss << std::fixed << std::setprecision(2) << compute_total(this->material_store);
                                this->material_total->set_text("Total: R " + moss.str());
                        }

                        std::ostringstream goss{""};
                        goss << std::fixed << std::setprecision(2) << this->compute_grand_total();
                        this->grand_total->set_text("Grand Total: R " + goss.str());
                }
                else
                {
                        columns->amount = 0;
                        entry->select_region(0, 15);
                        this->wrong_data_in_amount_column->show();
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
