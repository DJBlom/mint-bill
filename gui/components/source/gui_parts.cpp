#include <gui_parts.h>
#include <invoice_pdf.h>
#include <errors.h>
#include <iostream>
#include <invoice_data.h>




/***************************************************************************
 * Column Entries
 **************************************************************************/
Glib::RefPtr<gui::part::statement::columns::entries>
gui::part::statement::columns::entries::create()
{
	return Glib::make_refptr_for_instance<entries>(new entries());
}

Glib::RefPtr<gui::part::statement::columns::entries>
gui::part::statement::columns::entries::create(const data::invoice& _invoice)
{
	return Glib::make_refptr_for_instance<entries>(new entries(_invoice));
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

		label->set_xalign(0.50);
		label->set_hexpand(true);
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
	this->value = col->invoice.get_invoice_number();
	this->value.shrink_to_fit();
	label->set_text(this->value);
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

		label->set_xalign(0.50);
		label->set_hexpand(true);
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
	this->value = col->invoice.get_invoice_date();
	this->value.shrink_to_fit();
	label->set_text(this->value);
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

		label->set_xalign(0.5);
		label->set_hexpand(true);
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
	this->value = col->invoice.get_order_number();
	this->value.shrink_to_fit();
	label->set_text(this->value);
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
		check_button->set_label("Paid");
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

	this->value = col->invoice.get_paid_status();
	if (this->value == this->paid)
	{
		check_button->set_active(true);
	}
	else
	{
		check_button->set_active(false);
	}

	check_button->signal_toggled().connect([this, col, check_button]() {
		if (check_button->get_active())
		{
			col->invoice.set_paid_status(this->paid);
		}
		else
		{
			col->invoice.set_paid_status(this->not_paid);
		}
		this->value.clear();
		this->value = col->invoice.get_paid_status();
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
	this->value = col->invoice.get_grand_total();
	this->value.shrink_to_fit();
	label->set_text(this->value);
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
 * Row Item
 **************************************************************************/
	Glib::RefPtr<gui::part::statement::rows::invoice_pdf_entries>
gui::part::statement::rows::invoice_pdf_entries::create()
{
	return Glib::make_refptr_for_instance<invoice_pdf_entries>(new invoice_pdf_entries());
}

	Glib::RefPtr<gui::part::statement::rows::invoice_pdf_entries>
gui::part::statement::rows::invoice_pdf_entries::create(const data::pdf_invoice& _pdf_invoice)
{
	return Glib::make_refptr_for_instance<invoice_pdf_entries>(new invoice_pdf_entries(_pdf_invoice));
}


	Glib::RefPtr<gui::part::statement::rows::statement_pdf_entries>
gui::part::statement::rows::statement_pdf_entries::create()
{
	return Glib::make_refptr_for_instance<statement_pdf_entries>(new statement_pdf_entries());
}

	Glib::RefPtr<gui::part::statement::rows::statement_pdf_entries>
gui::part::statement::rows::statement_pdf_entries::create(const data::pdf_statement& _pdf_statement)
{
	return Glib::make_refptr_for_instance<statement_pdf_entries>(new statement_pdf_entries(_pdf_statement));
}


/***************************************************************************
 * Column View
 **************************************************************************/
gui::part::statement::column_view::column_view(const std::string& _name, const std::string& _vadjustment)
	: name{_name}, vadjustment{_vadjustment}
{
	name.shrink_to_fit();
	vadjustment.shrink_to_fit();
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
		this->adjustment = std::shared_ptr<Gtk::Adjustment>{
			_ui_builder->get_object<Gtk::Adjustment>(this->vadjustment)};
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

bool gui::part::statement::column_view::add_column(const interface::column_item& _item)
{
	bool added{false};
	if (_item.is_not_valid())
	{
		syslog(LOG_CRIT, "The item is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		added = true;
		this->view->append_column(_item.retrieve_item());
	}

	return added;
}

bool gui::part::statement::column_view::populate(const std::vector<std::any>& _invoices)
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
		success = true;
		this->store->remove_all();
		for (const std::any& invoice : _invoices)
		{
			data::invoice data{std::any_cast<data::invoice>(invoice)};
			if (data.is_valid() == false)
			{
				syslog(LOG_CRIT, "The data is not valid - "
						"filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				this->store->append(statement::columns::entries::create(data));
				Glib::signal_timeout().connect_once([this]() {
					this->adjustment->set_value(this->adjustment->get_upper());
				}, DURATION::MS_30);
			}
		}
	}

	return success;
}

bool gui::part::statement::column_view::clear()
{
	bool success{false};
	if (is_not_valid() == true)
	{
		syslog(LOG_CRIT, "The column_view is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		this->store->remove_all();
		if (this->store->property_n_items() <= 0)
		{
			success = true;
		}
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
				records.emplace_back(std::any(item->invoice));
			}
		}
	}

	return records;
}




/*************************************************************************************
 * PDF_WINDOW
 *************************************************************************************/
gui::part::statement::pdf_window::pdf_window(const std::string& _title) : title{_title}
{
	this->title.shrink_to_fit();
}

bool gui::part::statement::pdf_window::generate(const std::shared_ptr<poppler::document>& _document)
{
	bool success{false};
	if (!_document)
	{
		syslog(LOG_CRIT, "The poppler document is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		auto window = Gtk::make_managed<Gtk::Window>();
		auto scroller = Gtk::make_managed<Gtk::ScrolledWindow>();
		auto vbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 10);
		if (!(window || scroller || vbox))
		{
			syslog(LOG_CRIT, "The window, scroller, or vbox is not valid - "
					"filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = true;
			for (int i = 0; i < _document->pages(); ++i)
			{
				auto page_view = Gtk::make_managed<pdf_draw>(_document, i);
				if (!page_view)
				{
					syslog(LOG_CRIT, "The page_view is not valid - "
							"filename %s, line number %d", __FILE__, __LINE__);
					break;
				}
				page_view->set_content_width(PAGE_WIDTH);
				page_view->set_content_height(PAGE_HEIGHT);
				vbox->append(*page_view);
			}

			scroller->set_policy(Gtk::PolicyType::AUTOMATIC, Gtk::PolicyType::AUTOMATIC);
			scroller->set_child(*vbox);
			window->set_title(this->title);
			window->set_default_size(WIDTH, HEIGHT);
			window->set_child(*scroller);
			window->set_modal(true);
			window->present();
		}
	}

	return success;
}


/*************************************************************************************
 * PDF_DRAWER
 *************************************************************************************/
gui::part::statement::pdf_draw::pdf_draw(std::shared_ptr<poppler::document> _doc, int _page_num)
	: document(std::move(_doc)), page_number(_page_num)
{
	set_draw_func(sigc::mem_fun(*this, &pdf_draw::on_draw));
}

void gui::part::statement::pdf_draw::on_draw(const Cairo::RefPtr<Cairo::Context>& _cr, int _width, int _height)
{
	if (!this->document)
	{
		syslog(LOG_CRIT, "The poppler document is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
		return;
	}

	auto page = this->document->create_page(this->page_number);
	if (!page)
	{
		syslog(LOG_CRIT, "The poppler document page is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
		return;
	}

	poppler::page_renderer renderer{};
	renderer.set_render_hint(poppler::page_renderer::antialiasing, true);
	renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true);

	const double target_dpi = std::max(DPI::X, DPI::Y);
	auto image = renderer.render_page(page, target_dpi, target_dpi);
	if (!image.is_valid())
	{
		syslog(LOG_CRIT, "The poppler image is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
		return;
	}

	auto surface = Cairo::ImageSurface::create(
			reinterpret_cast<unsigned char*>(image.data()),
			Cairo::Surface::Format::ARGB32,
			image.width(), image.height(),
			image.bytes_per_row());

	const double scale_x = (static_cast<double>(_width) / image.width());
	const double scale_y = (static_cast<double>(_height) / image.height());
	const double scale = std::min(scale_x, scale_y);

	_cr->save();
	_cr->scale(scale, scale);
	_cr->set_source(surface, 0, 0);
	_cr->paint();
	_cr->restore();
}


/***************************************************************************
 * List View
 **************************************************************************/
gui::part::statement::invoice_pdf_view::invoice_pdf_view(const std::string& _name, const std::string& _vadjustment)
	: name{_name}, vadjustment_name{_vadjustment}
{
	name.shrink_to_fit();
	vadjustment_name.shrink_to_fit();
}

gui::part::statement::invoice_pdf_view::~invoice_pdf_view() {}

bool gui::part::statement::invoice_pdf_view::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (!_ui_builder)
	{
		syslog(LOG_CRIT, "The UI builder is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		this->view = std::unique_ptr<Gtk::ListView>{
			_ui_builder->get_widget<Gtk::ListView>(this->name)};
		this->store = std::shared_ptr<Gio::ListStore<rows::invoice_pdf_entries>>{
			Gio::ListStore<rows::invoice_pdf_entries>::create()};
		this->vadjustment = std::shared_ptr<Gtk::Adjustment>{
			_ui_builder->get_object<Gtk::Adjustment>(this->vadjustment_name)};
		Glib::RefPtr<Gtk::MultiSelection> model = Gtk::MultiSelection::create(this->store);
		Glib::RefPtr<Gtk::SignalListItemFactory> factory = Gtk::SignalListItemFactory::create();
		if (!this->view || !this->store || !model || !factory)
		{
			syslog(LOG_CRIT, "The view, store, model, or facotry are not valid - "
					"filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = true;
			this->view->set_model(model);
			this->view->set_factory(factory);
			this->view->set_single_click_activate(false);
			this->view->signal_activate().connect(sigc::mem_fun(*this, &invoice_pdf_view::display_invoice));
			factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &invoice_pdf_view::setup)));
			factory->signal_bind().connect(sigc::mem_fun(*this, &invoice_pdf_view::bind));
			factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &invoice_pdf_view::teardown)));
		}
	}

	return success;
}

bool gui::part::statement::invoice_pdf_view::is_not_valid() const
{
	return !(this->view || this->store);
}

bool gui::part::statement::invoice_pdf_view::populate(const std::vector<std::any>& _invoices)
{
	bool success{false};
	if (_invoices.empty() == true)
	{
		syslog(LOG_CRIT, "The _data is empty - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		this->store->remove_all();
		for (const std::any& invoice : _invoices)
		{
			data::pdf_invoice data{std::any_cast<data::pdf_invoice> (invoice)};
			if (data.is_valid() == false)
			{
				success = false;
				break;
			}
			else
			{
				success = true;
				this->store->append(rows::invoice_pdf_entries::create(data));
				Glib::signal_timeout().connect_once([this]() {
					this->vadjustment->set_value(this->vadjustment->get_upper());
				}, 30);
			}
		}
	}

	return success;
}

bool gui::part::statement::invoice_pdf_view::clear()
{
	bool success{true};
	if (this->is_not_valid())
	{
		syslog(LOG_CRIT, "The view or store is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
		success = false;
	}
	else
	{
		this->store->remove_all();
		if (this->store->property_n_items() > 0)
		{
			success = false;
		}
	}

	return success;
}

std::vector<std::any> gui::part::statement::invoice_pdf_view::extract()
{
	std::vector<std::any> records{};
	if (this->is_not_valid())
	{
		syslog(LOG_CRIT, "The view or store is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		for (guint i = 0; i < this->store->get_n_items(); ++i)
		{
			auto item = this->store->get_item(i);
			if (!item)
			{
				syslog(LOG_CRIT, "The item is not valid - "
						"filename %s, line number %d", __FILE__, __LINE__);
				break;
			}
			else
			{
				records.push_back(item->pdf_invoice);
			}
		}
	}

	return records;
}

void gui::part::statement::invoice_pdf_view::display_invoice(uint _position)
{
	auto item = this->store->get_item(_position);
	if (!item)
	{
		syslog(LOG_CRIT, "The item is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
		return;
	}

	auto data = std::dynamic_pointer_cast<rows::invoice_pdf_entries>(item);
	if (!data)
	{
		syslog(LOG_CRIT, "The data is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
		return;
	}

	data::pdf_invoice pdf_invoice{data->pdf_invoice};
	if (pdf_invoice.is_valid() == false)
	{
		syslog(LOG_CRIT, "The invoice data is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
		return;
	}

	feature::invoice_pdf invoice_pdf{};
	std::shared_ptr<poppler::document> document{};
	std::string raw_pdf = invoice_pdf.generate(pdf_invoice);
	std::vector<char> byte_vector{raw_pdf.begin(), raw_pdf.end()};
	poppler::byte_array byte_array{byte_vector};
	poppler::document* raw_doc = poppler::document::load_from_data(&byte_array);
	document = std::shared_ptr<poppler::document>(raw_doc, [](poppler::document* ptr) {
		if (ptr) {
			delete ptr;
		}
	});

	if (!document)
	{
		syslog(LOG_CRIT, "The poppler document is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
		return;
	}

	pdf_window pdf_window{"Invoice"};
	if (pdf_window.generate(document) == false)
	{
		syslog(LOG_CRIT, "Failed generate pdf_window - "
				"filename %s, line number %d", __FILE__, __LINE__);
		return;
	}
}

void gui::part::statement::invoice_pdf_view::setup(const Glib::RefPtr<Gtk::ListItem>& _item)
{
	if (!_item)
	{
		syslog(LOG_CRIT, "The list_item is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
		return;
	}
	else
	{
		auto label = Gtk::make_managed<Gtk::Label>("");
		if (!label)
		{
			syslog(LOG_CRIT, "The label is not valid - "
					"filename %s, line number %d", __FILE__, __LINE__);
			return;
		}

		label->set_xalign(0.50);
		label->set_hexpand(true);
		label->set_halign(Gtk::Align::START);
		_item->set_child(*label);
	}
}

void gui::part::statement::invoice_pdf_view::bind(const Glib::RefPtr<Gtk::ListItem>& _item)
{
	if (!_item)
	{
		syslog(LOG_CRIT, "The _item is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
		return;
	}

	auto data = std::dynamic_pointer_cast<rows::invoice_pdf_entries>(_item->get_item());
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

	data::pdf_invoice pdf_invoice{data->pdf_invoice};
	data::invoice invoice{pdf_invoice.get_invoice()};
	std::string details{"# " + invoice.get_invoice_number() + ", " + invoice.get_business_name() + ", " + invoice.get_invoice_date() + " "};
	label->set_text(details);
}

void gui::part::statement::invoice_pdf_view::teardown(const Glib::RefPtr<Gtk::ListItem>& _item)
{
	if (!_item)
	{
		syslog(LOG_CRIT, "The list_item is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		_item->unset_child();
	}
}





gui::part::statement::statement_pdf_view::statement_pdf_view(const std::string& _name, const std::string& _vadjustment)
	: name{_name}, vadjustment_name{_vadjustment}
{
	name.shrink_to_fit();
	vadjustment_name.shrink_to_fit();
}

gui::part::statement::statement_pdf_view::~statement_pdf_view() {}

bool gui::part::statement::statement_pdf_view::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (!_ui_builder)
	{
		syslog(LOG_CRIT, "The UI builder is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		this->view = std::unique_ptr<Gtk::ListView>{
			_ui_builder->get_widget<Gtk::ListView>(this->name)};
		this->store = std::shared_ptr<Gio::ListStore<rows::statement_pdf_entries>>{
			Gio::ListStore<rows::statement_pdf_entries>::create()};
		this->vadjustment = std::shared_ptr<Gtk::Adjustment>{
			_ui_builder->get_object<Gtk::Adjustment>(this->vadjustment_name)};
		Glib::RefPtr<Gtk::MultiSelection> model = Gtk::MultiSelection::create(this->store);
		Glib::RefPtr<Gtk::SignalListItemFactory> factory = Gtk::SignalListItemFactory::create();
		if (!this->view || !this->store || !model || !factory)
		{
			syslog(LOG_CRIT, "The view, store, model, or facotry are not valid - "
					"filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = true;
			this->view->set_model(model);
			this->view->set_factory(factory);
			this->view->set_single_click_activate(false);
			this->view->signal_activate().connect(sigc::mem_fun(*this, &statement_pdf_view::edit_statement));
			model->signal_selection_changed().connect(sigc::mem_fun(*this, &statement_pdf_view::selected_statement));
			factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &statement_pdf_view::setup)));
			factory->signal_bind().connect(sigc::mem_fun(*this, &statement_pdf_view::bind));
			factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &statement_pdf_view::teardown)));
		}
	}

	return success;
}

bool gui::part::statement::statement_pdf_view::is_not_valid() const
{
	return !(this->view || this->store);
}

bool gui::part::statement::statement_pdf_view::populate(const std::vector<std::any>& _statements)
{
	bool success{false};
	if (_statements.empty() == true)
	{
		syslog(LOG_CRIT, "The _data is empty - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		this->store->remove_all();
		for (const std::any& statement : _statements)
		{
			data::pdf_statement data{std::any_cast<data::pdf_statement> (statement)};
			if (data.is_valid() == false)
			{
				success = false;
				syslog(LOG_CRIT, "The _data is not valid - "
						"filename %s, line number %d", __FILE__, __LINE__);
				break;
			}
			else
			{
				success = true;
				this->store->append(rows::statement_pdf_entries::create(data));
				Glib::signal_timeout().connect_once([this]() {
					this->vadjustment->set_value(this->vadjustment->get_upper());
				}, 30);
			}
		}
	}

	return success;
}

bool gui::part::statement::statement_pdf_view::clear()
{
	bool success{false};
	if (this->is_not_valid())
	{
		syslog(LOG_CRIT, "The view or store is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		this->store->remove_all();
		if (this->store->property_n_items() <= 0)
		{
			success = true;
		}
	}

	return success;
}

std::vector<std::any> gui::part::statement::statement_pdf_view::extract()
{
	std::vector<std::any> records{};
	if (this->is_not_valid())
	{
		syslog(LOG_CRIT, "The view or store is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		for (guint i = 0; i < store->get_n_items(); ++i)
		{
			auto item = this->store->get_item(i);
			if (!item)
			{
				syslog(LOG_CRIT, "The item is not valid - "
						"filename %s, line number %d", __FILE__, __LINE__);
				break;
			}
			else
			{
				records.push_back(item->pdf_statement);
			}
		}
	}

	return records;
}

bool gui::part::statement::statement_pdf_view::double_click(std::function<void(const std::any&)> _callback)
{
	bool success{false};
	if (!_callback)
	{
		syslog(LOG_CRIT, "The _callback is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->double_click_callback = std::move(_callback);
	}

	return success;
}

bool gui::part::statement::statement_pdf_view::single_click(std::function<void(const std::vector<std::any>&)> _callback)
{
	bool success{false};
	if (!_callback)
	{
		syslog(LOG_CRIT, "The _callback is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->single_click_callback = std::move(_callback);
	}

	return success;
}

void gui::part::statement::statement_pdf_view::edit_statement(uint _position)
{
	auto item = this->store->get_item(_position);
	if (!item)
	{
		syslog(LOG_CRIT, "The item is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
		return;
	}

	auto data = std::dynamic_pointer_cast<rows::statement_pdf_entries>(item);
	if (!data)
	{
		syslog(LOG_CRIT, "The data is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
		return;
	}

	this->double_click_callback(std::any(data->pdf_statement));
}

void gui::part::statement::statement_pdf_view::selected_statement(uint _position, uint _items_selected)
{
	(void) _position;
	(void) _items_selected;

	Glib::RefPtr<Gtk::SelectionModel> model = this->view->get_model();
	if (!model)
	{
		syslog(LOG_CRIT, "The model is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
		return;
	}

	std::vector<std::any> selected_pdf_statements{};
	Glib::RefPtr<const Gtk::Bitset> items_list{model->get_selection()};
	for (guint index : *items_list)
	{
		auto item = this->store->get_item(index);
		if (!item)
		{
			syslog(LOG_CRIT, "The item is not valid - "
					"filename %s, line number %d", __FILE__, __LINE__);
			break;
		}
		auto data = std::dynamic_pointer_cast<rows::statement_pdf_entries>(item);
		if (!data)
		{
			syslog(LOG_CRIT, "The data is not valid - "
					"filename %s, line number %d", __FILE__, __LINE__);
			break;
		}

		selected_pdf_statements.emplace_back(data->pdf_statement);
	}

	this->single_click_callback(selected_pdf_statements);
}

void gui::part::statement::statement_pdf_view::setup(const Glib::RefPtr<Gtk::ListItem>& _item)
{
	if (!_item)
	{
		syslog(LOG_CRIT, "The list_item is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		auto label = Gtk::make_managed<Gtk::Label>("");
		if (!label)
		{
			syslog(LOG_CRIT, "The label is not valid - "
					"filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			label->set_xalign(0.50);
			label->set_hexpand(true);
			label->set_halign(Gtk::Align::START);
			_item->set_child(*label);
		}
	}
}

void gui::part::statement::statement_pdf_view::bind(const Glib::RefPtr<Gtk::ListItem>& _item)
{
	if (!_item)
	{
		syslog(LOG_CRIT, "The _item is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
		return;
	}

	auto data = std::dynamic_pointer_cast<rows::statement_pdf_entries>(_item->get_item());
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

	data::pdf_statement pdf_statement{data->pdf_statement};
	data::pdf_invoice pdf_invoice{(pdf_statement.get_pdf_invoices()).front()};
	data::client client{pdf_invoice.get_client()};
	std::string details{"# " + pdf_statement.get_number() + ", " + client.get_name() + ", " + pdf_statement.get_date()};
	label->set_text(details);
}

void gui::part::statement::statement_pdf_view::teardown(const Glib::RefPtr<Gtk::ListItem>& _item)
{
	if (!_item)
	{
		syslog(LOG_CRIT, "The list_item is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		_item->unset_child();
	}
}


/***************************************************************************
 * Search Bar
 **************************************************************************/
gui::part::search_bar::search_bar(const std::string& _search_bar_name)
	: search_bar_name{_search_bar_name}
{
	search_bar_name.shrink_to_fit();

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
			_ui_builder->get_widget<Gtk::SearchEntry>(this->search_bar_name)};
		if (this->is_not_valid())
		{
			syslog(LOG_CRIT, "The gui_search_bar is not valid - "
					"filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = true;
			this->gui_search_bar->signal_search_changed()
				.connect(sigc::mem_fun(*this, &search_bar::on_search_changed));
		}
	}

	return success;
}

bool gui::part::search_bar::update(const std::string& _current_stack_page_name)
{
	bool updated{false};
	if (_current_stack_page_name.empty())
	{
		syslog(LOG_CRIT, "The _current_stack_page_name is empty - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		updated = true;
		this->gui_search_bar->set_text("");
		this->stack_page_name = _current_stack_page_name;
	}

	return updated;
}

bool gui::part::search_bar::is_not_valid() const
{
	return !(this->gui_search_bar);
}

bool gui::part::search_bar::subscribe(const std::string& _page_name,
		std::function<void(const std::string&)> _callback) const
{
	bool success{false};
	if (_page_name.empty() || !_callback)
	{
		syslog(LOG_CRIT, "The _page_name or _callback is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		std::pair<
			std::unordered_map<
			std::string,
			std::function<void(const std::string&)>>::iterator,
			bool> result{this->subscribers.emplace(_page_name, _callback)};
		if (result.second)
		{
			success = true;
		}
		else
		{
			result.first->second = _callback;
		}
	}

	return success;
}

void gui::part::search_bar::on_search_changed()
{
	if (this->is_not_valid())
	{
		syslog(LOG_CRIT, "The search_bar is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		const std::string keyword{this->gui_search_bar->get_text()};
		for (const auto& [key, callback] : this->subscribers)
		{
			if (!callback)
			{
				syslog(LOG_CRIT, "The subscriber has not been registered - "
						"filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				callback(keyword);
			}
		}
	}
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
		if (this->is_not_valid())
		{
			syslog(LOG_CRIT, "The gui_dialog is not valid - "
					"filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = true;
			this->gui_dialog->signal_response().connect(sigc::mem_fun(*this, &dialog::on_response));
		}
	}

	return success;
}

bool gui::part::dialog::is_not_valid() const
{
	return !(this->gui_dialog);
}

bool gui::part::dialog::connect(const std::function<void(const int&)>& _callback)
{
	bool success{false};
	if (!_callback)
	{
		syslog(LOG_CRIT, "The _callback is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->callback = std::move(_callback);
	}

	return success;
}

bool gui::part::dialog::show() const
{
	bool success{false};
	if (this->is_not_valid())
	{
		syslog(LOG_CRIT, "The gui_dialog is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->gui_dialog->show();
	}

	return success;
}

bool gui::part::dialog::hide() const
{
	bool success{false};
	if (this->is_not_valid())
	{
		syslog(LOG_CRIT, "The gui_dialog is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->gui_dialog->hide();
	}

	return success;
}

void gui::part::dialog::on_response(int response)
{
	this->callback(response);
}





/****************************************************************************************
 * Sub button
 ***************************************************************************************/
gui::part::sub_button::sub_button(const std::string& _button_name)
	: button_name{_button_name}
{
	button_name.shrink_to_fit();

}

gui::part::sub_button::~sub_button() {}

bool gui::part::sub_button::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (!_ui_builder)
	{
		syslog(LOG_CRIT, "The UI builder or stack is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->button = std::unique_ptr<Gtk::Button>{_ui_builder->get_widget<Gtk::Button>(this->button_name)};
		this->button->signal_clicked()
			.connect(sigc::mem_fun(*this, &sub_button::on_clicked));
	}

	return success;
}

bool gui::part::sub_button::is_not_valid() const
{
	return !(this->button);
}

bool gui::part::sub_button::subscribe(std::function<void(void)> _callback)
{
	bool success{false};
	if (!_callback)
	{
		syslog(LOG_CRIT, "The _callback is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->callback = _callback;
	}

	return success;
}

bool gui::part::sub_button::disable()
{
	bool success{false};
	if (is_not_valid())
	{
		syslog(LOG_CRIT, "The button is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->button->set_sensitive(false);
	}

	return success;
}

bool gui::part::sub_button::enable()
{
	bool success{false};
	if (is_not_valid())
	{
		syslog(LOG_CRIT, "The button is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->button->set_sensitive(true);
	}

	return success;
}

void gui::part::sub_button::on_clicked()
{
	if (this->is_not_valid())
	{
		syslog(LOG_CRIT, "The button is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		this->callback();
	}
}
