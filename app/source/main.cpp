/********************************************************************************
 * Contents: The main function
 * Author: Dawid Blom
 * Date: October 26, 2024
 *
 * Note:
 *******************************************************************************/
#include <gtkmm.h>
#include <main_window.h>

int main(int argc, char** argv)
{
        gui::main_window window;
        Glib::RefPtr<Gtk::Application> app;
        app = Gtk::Application::create("org.engineering.admin-system");
        app->signal_activate().connect([&] () { window.setup(app); });

        return app->run(argc, argv);
}
