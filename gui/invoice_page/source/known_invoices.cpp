/********************************************************
 * Contents: Known Invoices class implementation
 * Author: Dawid J. Blom
 * Date: January 24, 2025
 *
 * NOTE:
 *******************************************************/
#include <known_invoices.h>

layout::known_invoices::known_invoices()
{
        this->dispatcher.connect(sigc::mem_fun(*this, &known_invoices::email_sent));
}

layout::known_invoices::~known_invoices() {}

bool layout::known_invoices::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        bool created{false};
        if (_ui_builder)
        {
                created = true;
                create_ui(_ui_builder);
                connect_email_alert();
                connect_email_button();
                connect_invoice_view();
                connect_no_internet_alert();
        }

        return created;
}

void layout::known_invoices::send_email(const data::invoice& _data)
{
        if (_data.is_valid())
        {
                std::thread([this, _data] () {
                        bool success = client_invoice.send_email(_data);
                        this->email_success = success;
                        this->dispatcher.emit();
                }).detach();
        }
}

void layout::known_invoices::populate(const std::string& _business_name)
{
        this->invoice_store->remove_all();
        if (_business_name.empty())
        {
                this->email_button->set_sensitive(false);
        }
        else
        {
                std::vector<data::invoice> db_invoices{client_invoice.search(_business_name, this->db)};
                populate_list_store(db_invoices);
        }
}

void layout::known_invoices::create_ui(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        this->invoice_view = std::unique_ptr<Gtk::ListView>{
                _ui_builder->get_widget<Gtk::ListView>("known-invoice-view")};
        this->invoices_adjustment = std::shared_ptr<Gtk::Adjustment>{
                _ui_builder->get_object<Gtk::Adjustment>("known-invoice-adjustment")};
        this->email_no_internet = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-email-no-internet-alert")};
        this->email_confirmation = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-email-alert")};
        this->email_button = std::unique_ptr<Gtk::Button>{
                _ui_builder->get_widget<Gtk::Button>("known-invoice-email-button")};
}

void layout::known_invoices::connect_email_alert()
{
        this->email_confirmation->signal_response().connect([this] (int response) {
                data::invoice data{this->current_invoice};
                if (response == GTK_RESPONSE_YES)
                {
                        this->email_confirmation->hide();
                        this->send_email(data);
                }
                else
                {
                        this->email_confirmation->hide();
                }
        });
}

void layout::known_invoices::connect_email_button()
{
        if (this->email_button)
        {
                this->email_button->set_sensitive(false);
                this->email_button->signal_clicked().connect([this] () {
                        this->email_confirmation->show();
                });
        }
}

void layout::known_invoices::connect_invoice_view()
{
        this->invoice_store = std::shared_ptr<Gio::ListStore<invoice_entries>>{
                Gio::ListStore<invoice_entries>::create()};
        Glib::RefPtr<Gtk::SingleSelection> selection_model = Gtk::SingleSelection::create(this->invoice_store);
        this->invoice_view->set_model(selection_model);
        this->invoice_view->set_name("invoice_view");

        connect_invoice_list_store();
        invoices(this->invoice_view);
}

void layout::known_invoices::connect_no_internet_alert()
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

void layout::known_invoices::connect_invoice_list_store()
{
        this->invoice_view->set_single_click_activate(true);
        this->invoice_view->signal_activate().connect(
                sigc::mem_fun(*this, &known_invoices::selected_invoice));
}

void layout::known_invoices::invoices(const std::unique_ptr<Gtk::ListView>& view)
{
        Glib::RefPtr<Gtk::SignalListItemFactory> factory = Gtk::SignalListItemFactory::create();
        factory->signal_setup().connect(sigc::bind(sigc::mem_fun(*this, &known_invoices::invoice_setup)));
        factory->signal_bind().connect(sigc::mem_fun(*this, &known_invoices::bind_invoices));
        factory->signal_teardown().connect(sigc::bind(sigc::mem_fun(*this, &known_invoices::invoice_teardown)));
        view->set_factory(factory);
}

void layout::known_invoices::invoice_setup(const Glib::RefPtr<Gtk::ListItem>& _item)
{
        auto label = Gtk::make_managed<Gtk::Label>("", Gtk::Align::START);
        label->set_hexpand(true);
        label->set_vexpand(false);
        label->set_size_request(200, 35);
        _item->set_child(*label);
}

void layout::known_invoices::invoice_teardown(const Glib::RefPtr<Gtk::ListItem>& _item)
{
        _item->unset_child();
}

void layout::known_invoices::bind_invoices(const Glib::RefPtr<Gtk::ListItem>& _item)
{
        auto data = std::dynamic_pointer_cast<invoice_entries>(_item->get_item());
        if (!data)
                return;

        auto label = dynamic_cast<Gtk::Label*>(_item->get_child());
        if (!label)
                return;

        data::invoice invoice{data->invoice};
        std::string details{"# " + invoice.get_invoice_number() + ", " + invoice.get_business_name() + ", " + invoice.get_invoice_date() + " "};
        label->set_text(details);
}

void layout::known_invoices::populate_list_store(const std::vector<data::invoice>& _invoices)
{
        for (const auto& invoice : _invoices)
        {
                if (invoice.is_valid())
                {
                        this->invoice_store->append(invoice_entries::create(invoice));
                }
                else
                {
                        Glib::signal_timeout().connect_once([this]() {
                                this->invoices_adjustment->set_value(this->invoices_adjustment->get_upper());
                        }, 30);
                }
        }
}

void layout::known_invoices::selected_invoice(uint position)
{
    auto item = this->invoice_store->get_item(position);
    auto data = std::dynamic_pointer_cast<invoice_entries>(item);
    if (!data)
        return;

    this->email_button->set_sensitive(true);
    this->current_invoice = data->invoice;
}

void layout::known_invoices::email_sent()
{
        if (this->email_success == false)
        {
                this->email_no_internet->show();
        }
}
