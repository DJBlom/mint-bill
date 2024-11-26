/********************************************************************************
 * Contents: The main function
 * Author: Dawid Blom
 * Date: October 26, 2024
 *
 * Note:
 *******************************************************************************/
#include <gtkmm.h>
#include <admin_system.h>





int main(int argc, char** argv)
{
        app::admin_system admin_system;
        Glib::RefPtr<Gtk::Application> app;
        app = Gtk::Application::create("org.engineering.admin-system");
        app->signal_activate().connect([&] () { admin_system.start(app); });

        return app->run(argc, argv);
}
