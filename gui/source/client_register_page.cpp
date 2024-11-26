/********************************************************
 * Contents: Client Register Page class implementation
 * Author: Dawid J. Blom
 * Date: November 26, 2024
 *
 * NOTE:
 *******************************************************/
#include <client_register_page.h>

gui::client_register_page::~client_register_page()
{
}

bool gui::client_register_page::create(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool created{false};
        if (ui_builder)
        {
                created = true;
        }
        return created;
}
