/********************************************************
 * Contents: Main Window class implementation
 * Author: Dawid J. Blom
 * Date: November 13, 2024
 *
 * NOTE:
 *******************************************************/
#include <syslog.h>
#include <main_window.h>

//GCOVR_EXCL_START
bool gui::main_window::create(const Glib::RefPtr<Gtk::Builder>& ui_builder, const Glib::RefPtr<Gtk::Application>& app)
{
        bool created{false};
        this->window = std::shared_ptr<Gtk::Window> {ui_builder->get_widget<Gtk::Window>("main-window")};
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
                app->add_window(*this->window);
        }

        return created;
}

std::shared_ptr<Gtk::Window> gui::main_window::retrieve()
{
	return this->window;
}

void gui::main_window::setup_css()
{
        Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
        css_provider->load_from_path("gui/mint-bill.css");
        Glib::RefPtr<Gdk::Display> display = Gdk::Display::get_default();
        if (!display)
        {
                syslog(LOG_CRIT, "Cannot get the display settings - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
                return;
        }

        Gtk::StyleContext::add_provider_for_display(
                display,
                css_provider,
                GTK_STYLE_PROVIDER_PRIORITY_USER
        );
}

//GCOVR_EXCL_STOP
