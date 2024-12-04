/***********************************************************
 * Contents: Invoice Page class implementation
 * Author: Dawid J. Blom
 * Date: November 28, 2024
 *
 * NOTE:
 **********************************************************/
#include <invoice_page.h>
#include <iostream>


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
        std::shared_ptr<Gtk::SearchEntry> search_entry{ui_builder->get_widget<Gtk::SearchEntry>("new-invoice-search-entry")};
        if (search_entry)
        {
                search_entry->signal_search_changed().connect([search_entry] () {
                        std::cout << "Searching: " << search_entry->get_text() << std::endl;
                });
        }
}

void gui::invoice_page::connect_save_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::Button> save_button{ui_builder->get_widget<Gtk::Button>("new-invoice-save-button")};
        if (save_button)
        {
                save_button->signal_clicked().connect([this] () {
                        this->save_alert_dialog->show();
                });
        }
}

void gui::invoice_page::connect_save_alert(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->save_alert_dialog = std::unique_ptr<Gtk::MessageDialog>(ui_builder->get_widget<Gtk::MessageDialog>("invoice-save-button-alert"));
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
        this->wrong_info_alert_dialog = std::unique_ptr<Gtk::MessageDialog>(ui_builder->get_widget<Gtk::MessageDialog>("invoice-wrong-info-alert"));
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

void gui::invoice_page::connect_description_view(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::ColumnView> view{ui_builder->get_widget<Gtk::ColumnView>("new-invoice-description")};
        this->description_store = Gio::ListStore<column_entries>::create();
        Glib::RefPtr<Gtk::MultiSelection> selection_model = Gtk::MultiSelection::create(this->description_store);
        view->set_model(selection_model);

        connect_description_add_button(ui_builder);
        connect_description_delete_button(selection_model, ui_builder);

        quantity_column(view);
        description_column(view);
        amount_column(view);
}

void gui::invoice_page::connect_description_add_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::Button> description_add_button{ui_builder->get_widget<Gtk::Button>("invoice-description-add-button")};
        Glib::RefPtr<Gtk::Adjustment> adjustment{ui_builder->get_object<Gtk::Adjustment>("new-invoice-description-column-view-adjustment")};
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
        std::unique_ptr<Gtk::Button> description_delete_button{ui_builder->get_widget<Gtk::Button>("invoice-description-delete-button")};
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

void gui::invoice_page::connect_material_view(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::ColumnView> view{ui_builder->get_widget<Gtk::ColumnView>("new-invoice-material")};
        this->material_store = Gio::ListStore<column_entries>::create();
        Glib::RefPtr<Gtk::MultiSelection> selection_model = Gtk::MultiSelection::create(this->material_store);
        view->set_model(selection_model);

        connect_material_add_button(ui_builder);
        connect_material_delete_button(selection_model, ui_builder);

        quantity_column(view);
        description_column(view);
        amount_column(view);
}

void gui::invoice_page::connect_material_add_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        std::unique_ptr<Gtk::Button> material_add_button{ui_builder->get_widget<Gtk::Button>("invoice-material-button")};
        Glib::RefPtr<Gtk::Adjustment> adjustment{ui_builder->get_object<Gtk::Adjustment>("new-invoice-material-column-view-adjustment")};
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
        std::unique_ptr<Gtk::Button> material_delete_button{ui_builder->get_widget<Gtk::Button>("invoice-material-delete-button")};
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
        std::reference_wrapper<Gtk::Entry> entry{*dynamic_cast<Gtk::Entry*>(list_item->get_child())};
        entry.get().signal_changed().connect([entry] {
                std::cout << "Quantity: " << entry.get().get_text() << std::endl;
        });
}

void gui::invoice_page::bind_description(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
        std::reference_wrapper<Gtk::Entry> entry{*dynamic_cast<Gtk::Entry*>(list_item->get_child())};
        entry.get().signal_changed().connect([entry] {
                std::cout << "Description: " << entry.get().get_text() << std::endl;
        });
}

void gui::invoice_page::bind_amount(const Glib::RefPtr<Gtk::ListItem>& list_item)
{
        std::reference_wrapper<Gtk::Entry> entry{*dynamic_cast<Gtk::Entry*>(list_item->get_child())};
        entry.get().signal_changed().connect([entry, this] {
                std::cout << "Amount: " << entry.get().get_text() << std::endl;
        });
}
