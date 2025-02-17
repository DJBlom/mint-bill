/********************************************************
 * Contents: Main Window class implementation
 * Author: Dawid J. Blom
 * Date: November 13, 2024
 *
 * NOTE:
 *******************************************************/
#include <main_window.h>


bool gui::main_window::create(const Glib::RefPtr<Gtk::Builder>& ui_builder, const Glib::RefPtr<Gtk::Application>& app)
{
        bool created{false};
        this->window = ui_builder->get_widget<Gtk::Window>("main-window");
        if (this->window)
        {
                created = true;
                setup_css();
                this->window->signal_hide().connect([this] () { delete this->window; });
                this->window->set_visible(true);
                this->connect_exit_button(ui_builder);
                this->connect_max_button(ui_builder);
                this->connect_min_button(ui_builder);
                app->add_window(*this->window);
        }

        return created;
}

void gui::main_window::connect_exit_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        Gtk::Button* exit_button{nullptr};
        exit_button = ui_builder->get_widget<Gtk::Button>("exit");
        if (exit_button)
        {
                exit_button->signal_clicked().connect([this] () { this->exit_clicked(); });
        }
}

void gui::main_window::connect_max_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        Gtk::Button* max_button{nullptr};
        max_button = ui_builder->get_widget<Gtk::Button>("maximize");
        if (max_button)
        {
                max_button->signal_clicked().connect([this] () { this->max_clicked(); });
        }
}

void gui::main_window::connect_min_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        Gtk::Button* min_button{nullptr};
        min_button = ui_builder->get_widget<Gtk::Button>("minimize");
        if (min_button)
        {
                min_button->signal_clicked().connect([this] () { this->min_clicked(); });
        }
}

void gui::main_window::exit_clicked()
{
        if (this->window)
        {
                this->window->close();
        }
        else
        {
                return;
        }
}

void gui::main_window::max_clicked()
{
        if ((this->window) && (this->window->is_maximized() == true))
        {
                this->window->unmaximize();
        }
        else if ((this->window) && (this->window->is_maximized() == false))
        {
                this->window->maximize();
        }
        else
        {
                return;
        }
}

void gui::main_window::min_clicked()
{
        if (window)
        {
                window->minimize();
        }
        else
        {
                return;
        }
}

void gui::main_window::setup_css()
{
        auto css_provider = Gtk::CssProvider::create();
        css_provider->load_from_path("gui/admin-system.css");
        auto display = Gdk::Display::get_default();
        if (display) {
                Gtk::StyleContext::add_provider_for_display(
                                display,
                                css_provider,
                                GTK_STYLE_PROVIDER_PRIORITY_USER
                                );
        }
}
