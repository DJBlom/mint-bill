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

namespace gui {
        //GCOVR_EXCL_START
        class main_window {
                public:
                        main_window() = default;
                        main_window(const main_window&) = delete;
                        main_window(main_window&&) = delete;
                        main_window& operator= (const main_window&) = delete;
                        main_window& operator= (main_window&&) = delete;
                        virtual ~main_window() = default;

                        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&, const Glib::RefPtr<Gtk::Application>&);

                private:
                        void connect_exit_button(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_max_button(const Glib::RefPtr<Gtk::Builder>&);
                        void connect_min_button(const Glib::RefPtr<Gtk::Builder>&);
                        void exit_clicked();
                        void min_clicked();
                        void max_clicked();
                        void setup_css();

                private:
                        std::unique_ptr<Gtk::Window> window{};
                        std::unique_ptr<Gtk::Button> max_button{};
                        std::unique_ptr<Gtk::Button> min_button{};
                        std::unique_ptr<Gtk::Button> exit_button{};
        };

        //GCOVR_EXCL_STOP
}
#endif
