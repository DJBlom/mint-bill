/********************************************************************************
 * Contents: The main function
 * Author: Dawid Blom
 * Date: October 26, 2024
 *
 * Note:
 *******************************************************************************/
#include <gtkmm.h>
#include <mint_bill.h>


int main(int argc, char** argv)
{
        app::mint_bill mint_bill;
        Glib::RefPtr<Gtk::Application> app;
        app = Gtk::Application::create("org.engineering.mint-bill");
        app->signal_activate().connect([&] () { mint_bill.start(app); });

        return app->run(argc, argv);
}
