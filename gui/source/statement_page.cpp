/********************************************************
 * Contents: Statement Page class declaration
 * Author: Dawid J. Blom
 * Date: February 19, 2025
 *
 * NOTE:
 *******************************************************/
#include <statement_page.h>

gui::statement_page::~statement_page() {}

bool gui::statement_page::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        bool created{false};
        if (_ui_builder)
        {
                created = true;
//                create_ui(_ui_builder);
        }

        return created;
}

//void layout::statement_page::create_ui(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
//{
//        this->invoice_view = std::unique_ptr<Gtk::ListView>{
//                _ui_builder->get_widget<Gtk::ListView>("known-invoice-view")};
//        this->invoices_adjustment = std::shared_ptr<Gtk::Adjustment>{
//                _ui_builder->get_object<Gtk::Adjustment>("known-invoice-adjustment")};
//        this->email_no_internet = std::unique_ptr<Gtk::MessageDialog>{
//                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-email-no-internet-alert")};
//        this->email_confirmation = std::unique_ptr<Gtk::MessageDialog>{
//                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-email-alert")};
//        this->email_button = std::unique_ptr<Gtk::Button>{
//                _ui_builder->get_widget<Gtk::Button>("known-invoice-email-button")};
//        this->print_confirmation = std::unique_ptr<Gtk::MessageDialog>{
//                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-print-alert")};
//        this->print_no_printer = std::unique_ptr<Gtk::MessageDialog>{
//                _ui_builder->get_widget<Gtk::MessageDialog>("invoice-print-no-printer-alert")};
//        this->print_button = std::unique_ptr<Gtk::Button>{
//                _ui_builder->get_widget<Gtk::Button>("known-invoice-print-button")};
//}
