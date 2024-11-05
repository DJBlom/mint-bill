/********************************************************
 * Contents: Admin class definition
 * Author: Dawid J. Blom
 * Date: October 26, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _ADMIN_H_
#define _ADMIN_H_
#include <gtkmm.h>

namespace app {
    class admin {
        public:
            admin() = default;
            admin(const admin&) = delete;
            admin(admin&&) = delete;
            admin& operator= (const admin&) = delete;
            admin& operator= (admin&&) = delete;
            ~admin() = default;

            bool run(int argc, char **argv)
            {
                app = Gtk::Application::create("org.engineering.admin");
                app->signal_activate().connect([this] () { app_active(); });

                return app->run(argc, argv);
            }

        private:
            void on_button_max_clicked()
            {
                if ((mainWindow) && (mainWindow->is_maximized() == true))
                {
                    mainWindow->unmaximize();
                }
                else if ((mainWindow) && (mainWindow->is_maximized() == false))
                {
                    mainWindow->maximize();
                }
            }

            void on_button_exit_clicked()
            {
                if (mainWindow)
                {
                    mainWindow->close();
                }
            }

            void on_button_min_clicked()
            {
                if (mainWindow)
                {
                    mainWindow->minimize();
                }
            }

            void app_active()
            {
                // Load the GtkBuilder file and instantiate its widgets:
                Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
                try
                {
                    refBuilder->add_from_file("/home/odin/workspace/software/invoice-system/gui/invoice-system.ui");
                }
                catch(const Glib::FileError& ex)
                {
                    std::cerr << "FileError: " << ex.what() << std::endl;
                    return;
                }
                catch(const Glib::MarkupError& ex)
                {
                    std::cerr << "MarkupError: " << ex.what() << std::endl;
                    return;
                }
                catch(const Gtk::BuilderError& ex)
                {
                    std::cerr << "BuilderError: " << ex.what() << std::endl;
                    return;
                }

                mainWindow = refBuilder->get_widget<Gtk::Window>("main-window");
                if (!mainWindow)
                {
                    std::cerr << "Could not get the dialog" << std::endl;
                    return;
                }

                // Get the GtkBuilder-instantiated button, and connect a signal handler:
                Gtk::Button *pButtonExit = refBuilder->get_widget<Gtk::Button>("exit");
                if (pButtonExit)
                    pButtonExit->signal_clicked().connect([this] () { on_button_exit_clicked(); });

                Gtk::Button *pButtonMax = refBuilder->get_widget<Gtk::Button>("maximize");
                if (pButtonMax)
                    pButtonMax->signal_clicked().connect([this] () { on_button_max_clicked(); });

                Gtk::Button *pButtonMin = refBuilder->get_widget<Gtk::Button>("minimize");
                if (pButtonMin)
                    pButtonMin->signal_clicked().connect([this] () { on_button_min_clicked(); });

                // It's not possible to delete widgets after app->run() has returned.
                // Delete the dialog with its child widgets before app->run() returns.
                mainWindow->signal_hide().connect([this] () { delete mainWindow; });
                app->add_window(*mainWindow);
                mainWindow->set_visible(true);
            }

        private:
            Gtk::Window* mainWindow{nullptr};
            Glib::RefPtr<Gtk::Application> app;
    };
}
#endif
