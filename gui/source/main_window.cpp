/********************************************************
 * Contents: Main Window class implementation
 * Author: Dawid J. Blom
 * Date: November 13, 2024
 *
 * NOTE:
 *******************************************************/
#include <syslog.h>
#include <main_window.h>

bool gui::main_window::create(const Glib::RefPtr<Gtk::Builder>& ui_builder, const Glib::RefPtr<Gtk::Application>& app)
{
        bool created{false};
        this->window = std::unique_ptr<Gtk::Window> {ui_builder->get_widget<Gtk::Window>("main-window")};
        if (!this->window)
        {
                syslog(LOG_CRIT, "The window is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
                created = true;
                setup_css();
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
        this->exit_button = std::unique_ptr<Gtk::Button>{ui_builder->get_widget<Gtk::Button>("exit")};
        if (!this->exit_button)
        {
                syslog(LOG_CRIT, "The exit button is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }
        else
        {
                this->exit_button->signal_clicked().connect([this] () {
                        this->exit_clicked();
                });
        }
}

void gui::main_window::connect_max_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->max_button = std::unique_ptr<Gtk::Button>{ui_builder->get_widget<Gtk::Button>("maximize")};
        if (!this->max_button)
        {
                syslog(LOG_CRIT, "The maximize button is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }
        else
        {
                this->max_button->signal_clicked().connect([this] () {
                        this->max_clicked();
                });
        }
}

void gui::main_window::connect_min_button(const Glib::RefPtr<Gtk::Builder>& ui_builder)
{
        this->min_button = std::unique_ptr<Gtk::Button>{ui_builder->get_widget<Gtk::Button>("minimize")};
        if (!this->min_button)
        {
                syslog(LOG_CRIT, "The minimize button is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }
        else
        {
                this->min_button->signal_clicked().connect([this] () {
                        this->min_clicked();
                });
        }
}

void gui::main_window::exit_clicked()
{
        if (!this->window)
        {
                syslog(LOG_CRIT, "The window is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }
        else
        {
                syslog(LOG_INFO, "User clicked the exit button");
                syslog(LOG_INFO, "Shutting down...");
                this->window->close();
        }
}

void gui::main_window::max_clicked()
{
        if (!this->window)
        {
                syslog(LOG_CRIT, "The window is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }
        else if ((this->window) && (this->window->is_maximized() == true))
        {
                syslog(LOG_INFO, "User clicked the maximized button");
                this->window->unmaximize();
        }
        else if ((this->window) && (this->window->is_maximized() == false))
        {
                syslog(LOG_INFO, "User clicked the maximized button");
                this->window->maximize();
        }
}

void gui::main_window::min_clicked()
{
        if (!this->window)
        {
                syslog(LOG_CRIT, "The window is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }
        else
        {
                syslog(LOG_INFO, "User clicked the minimize button");
                this->window->minimize();
        }
}

void gui::main_window::setup_css()
{
        Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
        css_provider->load_from_path("gui/admin-system.css");
        Glib::RefPtr<Gdk::Display> display = Gdk::Display::get_default();
        if (!display)
        {
                syslog(LOG_CRIT, "Cannot get the display settings - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }
        else
        {
                Gtk::StyleContext::add_provider_for_display(
                        display,
                        css_provider,
                        GTK_STYLE_PROVIDER_PRIORITY_USER
                );
        }
}
