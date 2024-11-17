/********************************************************
 * Contents: GUI interface definition
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _GUI_H_
#define _GUI_H_
#include <gtkmm.h>

namespace interface {
        class gui {
                public:
                        ~gui() = default;

                        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
        };
}
#endif
