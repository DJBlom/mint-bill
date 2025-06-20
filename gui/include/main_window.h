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
                        void setup_css();

                private:
                        std::unique_ptr<Gtk::Window> window{};
        };

        //GCOVR_EXCL_STOP
}
#endif
