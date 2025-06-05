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
        if (_ui_builder)
        {
                // save
                if (this->save_button.create(_ui_builder) == false)
                {
                        return false;
                }

                if (this->save_dialog.create(_ui_builder) == false)
                {
                        return false;
                }

                if (this->save_dialog.connect() == false)
                {
                        return false;
                }

                if (this->save_button.connect(this->save_dialog) == false)
                {
                        return false;
                }

                // print
                if (this->print_button.create(_ui_builder) == false)
                {
                        return false;
                }

                if (this->print_dialog.create(_ui_builder) == false)
                {
                        return false;
                }

                if (this->print_dialog.connect() == false)
                {
                        return false;
                }

                if (this->print_button.connect(this->print_dialog) == false)
                {
                        return false;
                }

                this->print_button.disable();

                // email
                if (this->email_button.create(_ui_builder) == false)
                {
                        return false;
                }

                if (this->email_dialog.create(_ui_builder) == false)
                {
                        return false;
                }

                if (this->email_dialog.connect() == false)
                {
                        return false;
                }

                if (this->email_button.connect(this->email_dialog) == false)
                {
                        return false;
                }

                this->email_button.disable();

//                create_ui(_ui_builder);
        }

        return true;
}

//void layout::statement_page::create_ui(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
//{
//        this->statement_view = std::unique_ptr<Gtk::ListView>{
//                _ui_builder->get_widget<Gtk::ListView>("known-invoice-view")};
//        this->statements_adjustment = std::shared_ptr<Gtk::Adjustment>{
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
