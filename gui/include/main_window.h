/********************************************************
 * Contents: Main Window class definition
 * Author: Dawid J. Blom
 * Date: November 14, 2024
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

                        [[nodiscard]] bool create(const Glib::RefPtr<Gtk::Builder>&, const Glib::RefPtr<Gtk::Application>&);

                private:
                        void connect_exit_button(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_max_button(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_min_button(const Glib::RefPtr<Gtk::Builder>&);
                        void exit_clicked();
                        void min_clicked();
                        void max_clicked();

                private:
                        Gtk::Window* window{nullptr};
        };
}
#endif
