/********************************************************
 * Contents: Main Window class definition
 * Author: Dawid J. Blom
 * Date: October 26, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_
#include <gtkmm.h>
#include <iostream>

namespace gui {
        class main_window {
                public:
                        main_window() = default;
                        main_window(const main_window&) = delete;
                        main_window(main_window&&) = delete;
                        main_window& operator= (const main_window&) = delete;
                        main_window& operator= (main_window&&) = delete;
                        ~main_window() = default;

                        void setup(const Glib::RefPtr<Gtk::Application>&);

                private:
                        [[nodiscard]] bool verify_ui_file();
                        [[nodiscard]] bool create_window();
                        void connect_exit_button();
                        void connect_max_button();
                        void connect_min_button();

                        void exit_clicked();
                        void min_clicked();
                        void max_clicked();

                private:
                        Gtk::Window* window{nullptr};
                        Glib::RefPtr<Gtk::Builder> ui_builder;
        };
}
#endif
