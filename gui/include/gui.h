/********************************************************
 * Contents: GUI interface definition
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _GUI_H_
#define _GUI_H_
#include <part.h>
#include <gtkmm.h>

namespace interface {
class gui {
public:
        virtual ~gui() = default;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&, const interface::observer&) = 0;
};

class gui_test {
public:
        virtual ~gui_test() = default;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
        [[nodiscard]] virtual bool search(const std::string&) = 0;
};
}
#endif
