
#include <sql.h>
#include <errors.h>
#include <iostream>
#include <gui_parts.h>
#include <statement_data.h>




/***************************************************************************
 * Column Entries
 **************************************************************************/
Glib::RefPtr<gui::part::statement::columns::entries>
gui::part::statement::columns::entries::create()
{
        return Glib::make_refptr_for_instance<entries>(new entries());
}

Glib::RefPtr<gui::part::statement::columns::entries>
gui::part::statement::columns::entries::create(const std::string& _invoice_number,
                                               const std::string& _date,
                                               const std::string& _order_number,
                                               const std::string& _paid_status,
                                               const std::string& _price)
{
        return Glib::make_refptr_for_instance<entries>(new entries(
                _invoice_number, _date, _order_number, _paid_status, _price));
}


/***************************************************************************
 * Column Item
 **************************************************************************/
gui::part::statement::columns::invoice_number::~invoice_number() {}

bool gui::part::statement::columns::invoice_number::create(const std::string& _title)
{
        this->factory = Gtk::SignalListItemFactory::create();
        if (!this->factory)
        {
                syslog(LOG_CRIT, "The factory is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		return false;
        }

        this->column = Gtk::ColumnViewColumn::create(_title, this->factory);
        if (!this->column)
        {
                syslog(LOG_CRIT, "The column is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		return false;
        }

        this->column->set_expand(true);
        this->factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &invoice_number::setup)));
        this->factory->signal_bind().connect(sigc::mem_fun(*this, &invoice_number::bind));
        this->factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &invoice_number::teardown)));

	return true;
}

bool gui::part::statement::columns::invoice_number::is_not_valid() const
{
        return !(this->column || this->factory);
}

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
		auto label = Gtk::make_managed<Gtk::Label>("Paid");
		if (!label)
		{
			syslog(LOG_CRIT, "The label is not valid - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			return;
		}

		label->set_xalign(0.50);                 // 0.0 = left, 1.0 = right
		label->set_hexpand(true);               // let it fill the header area
		label->set_halign(Gtk::Align::START);
                _item->set_child(*label);
        }
}

void gui::part::statement::columns::invoice_number::bind(const Glib::RefPtr<Gtk::ListItem>& _item)
{
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

gui::part::statement::columns::date::~date() {}

bool gui::part::statement::columns::date::create(const std::string& _title)
{
        this->factory = Gtk::SignalListItemFactory::create();
        if (!this->factory)
        {
                syslog(LOG_CRIT, "The factory is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		return false;
        }

        this->column = Gtk::ColumnViewColumn::create(_title, this->factory);
        if (!this->column)
        {
                syslog(LOG_CRIT, "The column is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		return false;
        }

        this->column->set_expand(true);
        this->factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &date::setup)));
        this->factory->signal_bind().connect(sigc::mem_fun(*this, &date::bind));
        this->factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &date::teardown)));

	return true;
}

bool gui::part::statement::columns::date::is_not_valid() const
{
        return !(this->column || this->factory);
}

Glib::RefPtr<Gtk::ColumnViewColumn> gui::part::statement::columns::date::retrieve_item() const
{
        return this->column;
}

std::string gui::part::statement::columns::date::retrieve_value() const
{
        return this->value;
}

void gui::part::statement::columns::date::setup(const Glib::RefPtr<Gtk::ListItem>& _item)
{
        if (!_item)
        {
                syslog(LOG_CRIT, "The list_item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }
        else
        {
		auto label = Gtk::make_managed<Gtk::Label>("Date");
		if (!label)
		{
			syslog(LOG_CRIT, "The label is not valid - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			return;
		}

		label->set_xalign(0.50);                 // 0.0 = left, 1.0 = right
		label->set_hexpand(true);               // let it fill the header area
		label->set_halign(Gtk::Align::START);
                _item->set_child(*label);
        }
}

void gui::part::statement::columns::date::bind(const Glib::RefPtr<Gtk::ListItem>& _item)
{
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

        this->value.clear();
        this->value = col->date;
        this->value.shrink_to_fit();
        label->set_text(col->date);
}

void gui::part::statement::columns::date::teardown(const Glib::RefPtr<Gtk::ListItem>& _item)
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

gui::part::statement::columns::order_number::~order_number() {}

bool gui::part::statement::columns::order_number::create(const std::string& _title)
{
        this->factory = Gtk::SignalListItemFactory::create();
        if (!this->factory)
        {
                syslog(LOG_CRIT, "The factory is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		return false;
        }

        this->column = Gtk::ColumnViewColumn::create(_title, this->factory);
        if (!this->column)
        {
                syslog(LOG_CRIT, "The column is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		return false;
        }

        this->column->set_expand(true);
        this->factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &order_number::setup)));
        this->factory->signal_bind().connect(sigc::mem_fun(*this, &order_number::bind));
        this->factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &order_number::teardown)));

	return true;
}

bool gui::part::statement::columns::order_number::is_not_valid() const
{
        return !(this->column || this->factory);
}

Glib::RefPtr<Gtk::ColumnViewColumn> gui::part::statement::columns::order_number::retrieve_item() const
{
        return this->column;
}

std::string gui::part::statement::columns::order_number::retrieve_value() const
{
        return this->value;
}

void gui::part::statement::columns::order_number::setup(const Glib::RefPtr<Gtk::ListItem>& _item)
{
        if (!_item)
        {
                syslog(LOG_CRIT, "The list_item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }
        else
        {
		auto label = Gtk::make_managed<Gtk::Label>("Order Number");
		if (!label)
		{
			syslog(LOG_CRIT, "The label is not valid - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			return;
		}

		label->set_xalign(0.5);                 // 0.0 = left, 1.0 = right
		label->set_hexpand(true);               // let it fill the header area
		label->set_halign(Gtk::Align::START);
                _item->set_child(*label);
        }
}

void gui::part::statement::columns::order_number::bind(const Glib::RefPtr<Gtk::ListItem>& _item)
{
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

        this->value.clear();
        this->value = col->order_number;
        this->value.shrink_to_fit();
        label->set_text(col->order_number);
}

void gui::part::statement::columns::order_number::teardown(const Glib::RefPtr<Gtk::ListItem>& _item)
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

bool gui::part::statement::columns::paid_status::create(const std::string& _title)
{
        this->factory = Gtk::SignalListItemFactory::create();
        if (!this->factory)
        {
                syslog(LOG_CRIT, "The factory is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		return false;
        }

        this->column = Gtk::ColumnViewColumn::create(_title, this->factory);
        if (!this->column)
        {
                syslog(LOG_CRIT, "The column is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		return false;
        }

        this->column->set_expand(true);
        this->factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &paid_status::setup)));
        this->factory->signal_bind().connect(sigc::mem_fun(*this, &paid_status::bind));
        this->factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &paid_status::teardown)));

	return true;
}

gui::part::statement::columns::paid_status::~paid_status() {}

bool gui::part::statement::columns::paid_status::is_not_valid() const
{
        return !(this->column || this->factory);
}

Glib::RefPtr<Gtk::ColumnViewColumn> gui::part::statement::columns::paid_status::retrieve_item() const
{
        return this->column;
}

std::string gui::part::statement::columns::paid_status::retrieve_value() const
{
        return this->value;
}

void gui::part::statement::columns::paid_status::setup(const Glib::RefPtr<Gtk::ListItem>& _item)
{
        if (!_item)
        {
                syslog(LOG_CRIT, "The list_item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }
        else
        {
                auto check_button = Gtk::make_managed<Gtk::CheckButton>();
		check_button->set_hexpand(true);
		check_button->set_halign(Gtk::Align::START);
		check_button->set_label("paid");
                _item->set_child(*check_button);
        }
}

void gui::part::statement::columns::paid_status::bind(const Glib::RefPtr<Gtk::ListItem>& _item)
{
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

	auto check_button = static_cast<Gtk::CheckButton*>(_item->get_child());
        if (!check_button)
        {
                syslog(LOG_CRIT, "The check_button is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

	this->value = col->paid_status;
	check_button->signal_toggled().connect([this, col, check_button]() {
		if (check_button->get_active())
		{
			col->paid_status = "paid";
		}
		else
		{
			col->paid_status = "not paid";
		}
		this->value.clear();
		this->value = col->paid_status;
		this->value.shrink_to_fit();
	});
}

void gui::part::statement::columns::paid_status::teardown(const Glib::RefPtr<Gtk::ListItem>& _item)
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

gui::part::statement::columns::price::~price() {}

bool gui::part::statement::columns::price::create(const std::string& _title)
{
        this->factory = Gtk::SignalListItemFactory::create();
        if (!this->factory)
        {
                syslog(LOG_CRIT, "The factory is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		return false;
        }

        this->column = Gtk::ColumnViewColumn::create(_title, this->factory);
        if (!this->column)
        {
                syslog(LOG_CRIT, "The column is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		return false;
        }

        this->column->set_expand(true);
        this->factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &price::setup)));
        this->factory->signal_bind().connect(sigc::mem_fun(*this, &price::bind));
        this->factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &price::teardown)));

	return true;
}
bool gui::part::statement::columns::price::is_not_valid() const
{
        return !(this->column || this->factory);
}

Glib::RefPtr<Gtk::ColumnViewColumn> gui::part::statement::columns::price::retrieve_item() const
{
        return this->column;
}

std::string gui::part::statement::columns::price::retrieve_value() const
{
        return this->value;
}

void gui::part::statement::columns::price::setup(const Glib::RefPtr<Gtk::ListItem>& _item)
{
        if (!_item)
        {
                syslog(LOG_CRIT, "The list_item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }
        else
        {
		auto label = Gtk::make_managed<Gtk::Label>("Price");
		if (!label)
		{
			syslog(LOG_CRIT, "The label is not valid - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			return;
		}

		label->set_xalign(0.5);
		label->set_hexpand(true);
		label->set_halign(Gtk::Align::START);
                _item->set_child(*label);
        }
}

void gui::part::statement::columns::price::bind(const Glib::RefPtr<Gtk::ListItem>& _item)
{
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

        this->value.clear();
        this->value = col->price;
        this->value.shrink_to_fit();
        label->set_text(col->price);
}

void gui::part::statement::columns::price::teardown(const Glib::RefPtr<Gtk::ListItem>& _item)
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
gui::part::statement::column_view::column_view(const std::string& _name) : name{_name}
{
        name.shrink_to_fit();
}

gui::part::statement::column_view::~column_view() {}

bool gui::part::statement::column_view::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
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
		this->adjustment = Gtk::Adjustment::create(0.0, 0, 0, 0, 0, 0);
                Glib::RefPtr<Gtk::MultiSelection> model = Gtk::MultiSelection::create(this->store);
                if (!this->view || !this->store || !model)
                {
                        syslog(LOG_CRIT, "The view, store, or model are not valid - "
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

bool gui::part::statement::column_view::is_not_valid() const
{
        return !(this->view || this->store);
}

bool gui::part::statement::column_view::add_column(const interface::item& _item)
{
        bool added{false};
        if (_item.is_not_valid())
        {
                syslog(LOG_CRIT, "The item is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
                this->view->append_column(_item.retrieve_item());
                added = true;
        }

        return added;
}

bool gui::part::statement::column_view::populate(const interface::feature& _feature)
{
	bool success{false};
	if (this->is_not_valid())
	{
		syslog(LOG_CRIT, "The view or store is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
		return success;
	}
	else
	{
		this->store->remove_all();
		std::vector<std::any> statements = _feature.load("test_business");
		for (const std::any& statement : statements)
		{
			data::statement data{std::any_cast<data::statement>(statement)};
			if (data.is_valid() == false)
			{
				syslog(LOG_CRIT, "The data is not valid - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				this->store->append(statement::columns::entries::create(
					data.get_invoice_number(),
					data.get_date(),
					data.get_order_number(),
					data.get_paid_status(),
					data.get_price()
				));
				Glib::signal_timeout().connect_once([this]() {
					this->adjustment->set_value(this->adjustment->get_upper());
				}, DURATION::MS_30);
			}
		}
		success = true;
	}

        return success;
}

std::vector<std::any> gui::part::statement::column_view::extract()
{
	std::vector<std::any> records{};
	if (this->is_not_valid())
	{
		syslog(LOG_CRIT, "The view or store is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
		return records;
	}
	else
	{
		for (guint i = 0; i < store->get_n_items(); ++i)
		{
			auto item = store->get_item(i);
			if (!item)
			{
				syslog(LOG_CRIT, "The item is not valid - "
						 "filename %s, line number %d", __FILE__, __LINE__);
				break;
			}
			else
			{
				data::statement data;
				data.set_invoice_number(item->invoice_number);
				data.set_date(item->date);
				data.set_order_number(item->order_number);
				data.set_paid_status(item->paid_status);
				data.set_price(item->price);
				records.push_back(data);
			}
		}
	}

	return records;
}




/***************************************************************************
 * Search Bar
 **************************************************************************/
gui::part::search_bar::search_bar(const std::string& _name) : name{_name}
{
        name.shrink_to_fit();
}

gui::part::search_bar::~search_bar() {}

bool gui::part::search_bar::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        bool success{false};
        if (!_ui_builder)
        {
                syslog(LOG_CRIT, "The UI builder is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
                this->gui_search_bar = std::unique_ptr<Gtk::SearchEntry>{
			_ui_builder->get_widget<Gtk::SearchEntry>(this->name)};
                if (this->gui_search_bar)
                {
                        success = true;
                }
        }

        return success;
}

bool gui::part::search_bar::is_not_valid() const
{
        return !(this->gui_search_bar);
}

bool gui::part::search_bar::connect()
{
        bool success{false};
	if (this->is_not_valid())
	{
		syslog(LOG_CRIT, "The search bar is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->gui_search_bar->signal_changed().connect([this] () {
			Glib::signal_timeout().connect_once([this](){
				this->search_value = this->gui_search_bar->get_text();
				std::cout << "++++++++++++++++++++++++ Searched for: " << this->search_value << std::endl;
			}, 200, Glib::PRIORITY_DEFAULT);
		});
	}

	return success;
}

std::string gui::part::search_bar::keyword()
{
	return "";
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
        return !(this->gui_dialog);
}

bool gui::part::dialog::connect() const
{
        bool success{false};
        if (this->is_not_valid())
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
