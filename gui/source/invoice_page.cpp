/***********************************************************
 * Contents: Invoice Page class implementation
 * Author: Dawid J. Blom
 * Date: November 28, 2024
 *
 * NOTE:
 **********************************************************/
#include <invoice_page.h>


gui::invoice_page::~invoice_page()
{

}

bool gui::invoice_page::create(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool created{false};
        if (verify_ui_builder(ui_builder) == true)
        {
                created = true;
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
