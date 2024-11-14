/********************************************************
 * Contents: Main Window class implementation
 * Author: Dawid J. Blom
 * Date: November 13, 2024
 *
 * NOTE:
 *******************************************************/
#include <main_window.h>


void gui::main_window::setup(const Glib::RefPtr<Gtk::Application>& app)
{
        if (this->verify_ui_file() == true)
        {
                if (this->create_window() == true)
                {
                        this->connect_exit_button();
                        this->connect_max_button();
                        this->connect_min_button();
                        app->add_window(*this->window);
                }
                else
                {
                        std::cerr << "Error: failed to load the main window" << std::endl;
                        return;
                }
        }
        else
        {
                std::cerr << "Error: failed to load the ui file" << std::endl;
                return;
        }
}

bool gui::main_window::verify_ui_file()
{
        bool verified{false};
        this->ui_builder = Gtk::Builder::create();
        try
        {
                verified = true;
                this->ui_builder->add_from_file("gui/admin-system.ui");
        }
        catch(const Glib::FileError& ex)
        {
                std::cerr << "FileError: " << ex.what() << std::endl;
                return verified;
        }
        catch(const Glib::MarkupError& ex)
        {
                std::cerr << "MarkupError: " << ex.what() << std::endl;
                return verified;
        }
        catch(const Gtk::BuilderError& ex)
        {
                std::cerr << "BuilderError: " << ex.what() << std::endl;
                return verified;
        }

        return verified;
}

bool gui::main_window::create_window()
{
        bool created{false};
        this->window = ui_builder->get_widget<Gtk::Window>("main-window");
        if (this->window)
        {
                created = true;
                this->window->signal_hide().connect([this] () { delete this->window; });
                this->window->set_visible(true);
        }

        return created;
}

void gui::main_window::connect_exit_button()
{
        Gtk::Button* exit_button{nullptr};
        exit_button = ui_builder->get_widget<Gtk::Button>("exit");
        if (exit_button)
        {
                exit_button->signal_clicked().connect([this] () { this->exit_clicked(); });
        }
}

void gui::main_window::connect_max_button()
{
        Gtk::Button* max_button{nullptr};
        max_button = ui_builder->get_widget<Gtk::Button>("maximize");
        if (max_button)
        {
                max_button->signal_clicked().connect([this] () { this->max_clicked(); });
        }
}

void gui::main_window::connect_min_button()
{
        Gtk::Button* min_button{nullptr};
        min_button = this->ui_builder->get_widget<Gtk::Button>("minimize");
        if (min_button)
        {
                min_button->signal_clicked().connect([this] () { this->min_clicked(); });
        }
}

void gui::main_window::exit_clicked()
{
        if (window)
        {
                window->close();
        }
        else
        {
                return;
        }
}

void gui::main_window::max_clicked()
{
        if ((window) && (window->is_maximized() == true))
        {
                window->unmaximize();
        }
        else if ((window) && (window->is_maximized() == false))
        {
                window->maximize();
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
