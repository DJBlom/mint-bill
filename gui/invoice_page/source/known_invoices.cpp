/********************************************************
 * Contents: Known Invoices class implementation
 * Author: Dawid J. Blom
 * Date: January 24, 2025
 *
 * NOTE:
 *******************************************************/
#include <known_invoices.h>
#include <pdf_invoice_data.h>
#include <iostream>

layout::known_invoices::known_invoices()
{
        this->email_dispatcher.connect(sigc::mem_fun(*this, &known_invoices::email_sent));
        this->print_dispatcher.connect(sigc::mem_fun(*this, &known_invoices::printed));
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
                connect_print_alert();
                connect_print_button();
                connect_invoice_view();
                connect_no_printer_alert();
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
                        this->email_dispatcher.emit();
                }).detach();
        }
}

void layout::known_invoices::on_draw_page(const Glib::RefPtr<Gtk::PrintContext>& context, int page_nr)
{
        if (!this->document || page_nr >= this->document->pages()) {
                return;
        }

        auto page = this->document->create_page(page_nr);
        poppler::page_renderer renderer;

        double dpi_x = 190;
        double dpi_y = 290;
        const double target_dpi = std::max(dpi_x, dpi_y);
        auto image = renderer.render_page(page, target_dpi, target_dpi);
        if (image.is_valid() == false)
                return;

        renderer.set_render_hint(poppler::page_renderer::antialiasing, true);
        renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true);

        auto cairo_surface = Cairo::ImageSurface::create(
                reinterpret_cast<unsigned char*> (image.data()), Cairo::Surface::Format::ARGB32,
                image.width(), image.height(),
                image.bytes_per_row());

        double sx = context->get_width() / image.width();
        double sy = context->get_height() / image.height();
        double scale_factor = std::min(sx, sy);
        auto cr = context->get_cairo_context();
        cr->save();
        cr->scale(scale_factor, scale_factor);
        cr->set_source(cairo_surface, 0, 0);
        cr->paint();
        cr->restore();
}

void layout::known_invoices::print_invoice(const data::invoice& _data)
{
        if (_data.is_valid())
        {
                std::thread([this, _data] () {
                        //this->document.reset(poppler::document::load_from_file("test/test2.pdf"));
                        data::pdf_invoice pdf_data{client_invoice.create_pdf_to_print(_data)};
                        std::shared_ptr<poppler::document> current_document{this->pdf.generate_for_print(pdf_data)};
                        this->document = std::move(current_document);
                        if (!this->document)
                        {
                                std::cerr << "Failed to load PDF document." << std::endl;
                                return;
                        }

                        Glib::RefPtr<Gtk::PrintOperation> print_operation = Gtk::PrintOperation::create();
                        if (!print_operation)
                                return;

                        auto page_setup = Gtk::PageSetup::create();
                        page_setup->set_orientation(Gtk::PageOrientation::PORTRAIT);
                        page_setup->set_paper_size(Gtk::PaperSize(Gtk::PAPER_NAME_A4));
                        page_setup->set_top_margin(0, Gtk::Unit::POINTS);
                        page_setup->set_bottom_margin(0, Gtk::Unit::POINTS);
                        page_setup->set_left_margin(0, Gtk::Unit::POINTS);
                        page_setup->set_right_margin(0, Gtk::Unit::POINTS);

                        print_operation->set_default_page_setup(page_setup);
                        print_operation->signal_draw_page().connect(
                                        sigc::mem_fun(*this, &known_invoices::on_draw_page));
                        print_operation->signal_done().connect(sigc::bind(
                                                sigc::mem_fun(*this, &known_invoices::on_printoperation_done), print_operation));
                        print_operation->set_n_pages(this->document->pages());
                        print_operation->set_unit(Gtk::Unit::POINTS);
                        print_operation->set_has_selection(false);
                        print_operation->set_use_full_page(true);
                        print_operation->set_show_progress(true);
                        print_operation->set_allow_async(true);

                        print_operation->run(Gtk::PrintOperation::Action::PREVIEW);
                        this->print_dispatcher.emit();
                }).detach();
        }
}

void layout::known_invoices::on_printoperation_done(Gtk::PrintOperation::Result result,
  const Glib::RefPtr<Gtk::PrintOperation>& op)
{
        if (result == Gtk::PrintOperation::Result::ERROR)
        {
                std::cerr << "Printing failed." << std::endl;
        }
        else if (result == Gtk::PrintOperation::Result::CANCEL)
        {
                std::cout << "Printing was cancelled." << std::endl;
        }
        else if (result == Gtk::PrintOperation::Result::IN_PROGRESS)
        {
                std::cout << "Printing was cancelled." << std::endl;
        }

        std::string has_printer{op->get_print_settings()->get_printer()};
        if (has_printer.empty())
        {
                this->print_success = false;
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
        this->print_confirmation = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-print-alert")};
        this->print_no_printer = std::unique_ptr<Gtk::MessageDialog>{
                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-print-no-printer-alert")};
        this->print_button = std::unique_ptr<Gtk::Button>{
                _ui_builder->get_widget<Gtk::Button>("known-invoice-print-button")};
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

void layout::known_invoices::connect_print_alert()
{
        this->print_confirmation->signal_response().connect([this] (int response) {
                data::invoice data{this->current_invoice};
                if (response == GTK_RESPONSE_YES)
                {
                        this->print_confirmation->hide();
                        this->print_invoice(data);
                }
                else
                {
                        this->print_confirmation->hide();
                }
        });
}

void layout::known_invoices::connect_print_button()
{
        if (this->print_button)
        {
                this->print_button->set_sensitive(false);
                this->print_button->signal_clicked().connect([this] () {
                        this->print_confirmation->show();
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

void layout::known_invoices::connect_no_printer_alert()
{
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
    this->print_button->set_sensitive(true);
    this->current_invoice = data->invoice;
}

void layout::known_invoices::email_sent()
{
        if (this->email_success == false)
        {
                this->email_no_internet->show();
        }
}

void layout::known_invoices::printed()
{
        if (this->print_success == false)
        {
                this->print_no_printer->show();
        }
}
