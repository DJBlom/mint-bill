#include <errors.h>
#include <gui_parts.h>



/***************************************************************************
 * Column Item
 **************************************************************************/
gui::part::statement::columns::invoice_number::invoice_number(const std::string& _title)
{
        this->factory = Gtk::SignalListItemFactory::create();
        if (!this->factory)
        {
                syslog(LOG_CRIT, "The factory is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                throw app::errors::construction;
        }

        this->column = Gtk::ColumnViewColumn::create(_title, this->factory);
        if (!this->column)
        {
                syslog(LOG_CRIT, "The column is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                throw app::errors::construction;
        }

        this->column->set_expand(false);
        this->column->set_expand(false);
        this->factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &invoice_number::setup)));
        this->factory->signal_bind().connect(sigc::mem_fun(*this, &invoice_number::bind));
        this->factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &invoice_number::teardown)));
}

gui::part::statement::columns::invoice_number::~invoice_number() {}

Glib::RefPtr<Gtk::ColumnViewColumn> gui::part::statement::columns::invoice_number::retrieve_item() const
{
        return this->column;
}

std::string gui::part::statement::columns::invoice_number::retrieve_value() const
{
        return this->value;
}

void gui::part::statement::columns::invoice_number::setup(const Glib::RefPtr<Gtk::ListItem>& _item)
{
        if (!_item)
        {
                syslog(LOG_CRIT, "The list_item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }
        else
        {
                _item->set_child(*Gtk::make_managed<Gtk::Label>("", Gtk::Align::FILL));
        }
}

void gui::part::statement::columns::invoice_number::bind(const Glib::RefPtr<Gtk::ListItem>& _item)
{
        bool success{false};
        if (!_item)
        {
                syslog(LOG_CRIT, "The list_item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        auto col = std::dynamic_pointer_cast<entries>(_item->get_item());
        if (!col)
        {
                syslog(LOG_CRIT, "The column is not valid - "
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
                syslog(LOG_CRIT, "The label is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);

        this->value.clear();
        this->value = col->invoice_number;
        this->value.shrink_to_fit();
        label->set_text(col->invoice_number);
}

void gui::part::statement::columns::invoice_number::teardown(const Glib::RefPtr<Gtk::ListItem>& _item)
{
        if (!_item)
        {
                syslog(LOG_CRIT, "The list_item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }
        else
        {
                _item->unset_child();
        }
}

/***************************************************************************
 * Column View
 **************************************************************************/
gui::part::column_view::column_view(const std::string& _name) : name{_name}
{
        name.shrink_to_fit();
}

gui::part::column_view::~column_view() {}

bool gui::part::column_view::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        bool success{false};
        if (!_ui_builder)
        {
                syslog(LOG_CRIT, "The UI builder is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
                this->view = std::unique_ptr<Gtk::ColumnView>{
                        _ui_builder->get_widget<Gtk::ColumnView>(this->name)};
                this->store = std::shared_ptr<Gio::ListStore<statement::columns::entries>>{
                        Gio::ListStore<statement::columns::entries>::create()};
                Glib::RefPtr<Gtk::MultiSelection> model = Gtk::MultiSelection::create(this->store);
                if (!this->view && !this->store && !model)
                {
                        syslog(LOG_CRIT, "The view, store, and model are not valid - "
                                         "filename %s, line number %d", __FILE__, __LINE__);
                }
                else
                {
                        success = true;
                        this->view->set_model(model);
                }
        }

        return success;
}

bool gui::part::column_view::is_not_valid() const
{
        return !(this->view && this->store);
}

bool gui::part::column_view::add_column(const interface::item& _item)
{
        bool added{true};

        this->view->append_column(_item.retrieve_item());

        return added;

//        this->description_store = std::shared_ptr<Gio::ListStore<column_entries>>{
//                Gio::ListStore<column_entries>::create()};
//        if (!this->description_store)
//        {
//                syslog(LOG_CRIT, "The description_store is not valid - "
//                                 "filename %s, line number %d", __FILE__, __LINE__);
//                return;
//        }
//
//        Glib::RefPtr<Gtk::MultiSelection> selection_model = Gtk::MultiSelection::create(this->description_store);
//        if (!selection_model)
//        {
//                syslog(LOG_CRIT, "The description selection_model is not valid - "
//                                 "filename %s, line number %d", __FILE__, __LINE__);
//                return;
//        }
//
//        this->description_view->set_model(selection_model);
//        this->description_view->set_name("description_view");
//
//        connect_description_add_button();
//        connect_description_delete_button(selection_model);
//        connect_description_list_store();
//
//        quantity_column(this->description_view);
//        description_column(this->description_view);
//        amount_column(this->description_view);
}





/***************************************************************************
 * Dialog
 **************************************************************************/
gui::part::dialog::dialog(const std::string& _name) : name{_name}
{
        name.shrink_to_fit();
}

gui::part::dialog::~dialog() {}

bool gui::part::dialog::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        bool success{false};
        if (!_ui_builder)
        {
                syslog(LOG_CRIT, "The UI builder is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
                this->gui_dialog = std::unique_ptr<Gtk::MessageDialog>{
                        _ui_builder->get_widget<Gtk::MessageDialog>(this->name)};
                if (this->gui_dialog)
                {
                        success = true;
                }
        }

        return success;
}

bool gui::part::dialog::is_not_valid() const
{
        return (!this->gui_dialog);
}

bool gui::part::dialog::connect() const
{
        bool success{false};
        if (!this->gui_dialog)
        {
                syslog(LOG_CRIT, "The dialog is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
                success = true;
                this->gui_dialog->signal_response().connect([this] (int response) {
                        switch (response)
                        {
                                case GTK_RESPONSE_YES:
                                        this->gui_dialog->hide();
                                        break;
                                case GTK_RESPONSE_NO:
                                        this->gui_dialog->hide();
                                        break;
                                default:
                                        this->gui_dialog->hide();
                                        break;
                        }
                });
        }

        return success;
}

void gui::part::dialog::show() const
{
        this->gui_dialog->show();
}








/***************************************************************************
 * Button
 **************************************************************************/
gui::part::button::button(const std::string& _name) : name{_name}
{
        name.shrink_to_fit();
}

gui::part::button::~button() {}

bool gui::part::button::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        bool created{false};
        if (!_ui_builder)
        {
                syslog(LOG_CRIT, "The UI builder is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
                this->gui_button = std::unique_ptr<Gtk::Button>{
                        _ui_builder->get_widget<Gtk::Button>(this->name)};
                if (this->gui_button != nullptr)
                {
                        created = true;
                }
        }

        return created;
}

bool gui::part::button::connect(const interface::dialog& _dialog)
{
        bool success{false};
        if (_dialog.is_not_valid())
        {
                syslog(LOG_CRIT, "The dialog is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
                success = true;
                this->gui_button->signal_clicked().connect([&] () {
                        _dialog.show();
                });
        }

        return success;
}

void gui::part::button::disable()
{
        this->gui_button->set_sensitive(false);
}

void gui::part::button::enable()
{
        this->gui_button->set_sensitive(true);
}
