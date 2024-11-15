/***********************************************************
 * Contents: Business Information Page class implementation
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 **********************************************************/
#include <business_info_page.h>


bool gui::business_info_page::create(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        bool created{false};
        this->business_name = ui_builder->get_widget<Gtk::Entry>(this->business_prefix + "-business-info-name-entry");
        if (this->business_name)
        {
                created = true;
                connect_save_button(ui_builder);
        }

        return created;
}

void gui::business_info_page::connect_save_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        Gtk::Button* save_button{nullptr};
        save_button = ui_builder->get_widget<Gtk::Button>(this->business_prefix + "-business-info-save-button");
        if (save_button)
        {
                save_button->signal_clicked().connect([this] () { this->save_clicked(); });
        }
}

void gui::business_info_page::save_clicked()
{
        if (this->business_name)
        {
                std::cout << "Business Name: " << this->business_name->get_text() << std::endl;
        }
        else
        {
                return;
        }
}
