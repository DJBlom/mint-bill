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

class register_page {
public:
        virtual ~register_page() = default;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
        [[nodiscard]] virtual bool search(const std::string&) = 0;
        [[nodiscard]] virtual bool save() = 0;
};

class operations_page {
public:
        virtual ~operations_page() = default;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&, const std::shared_ptr<Gtk::Window>&) = 0;
        [[nodiscard]] virtual bool search(const std::string&) = 0;
        [[nodiscard]] virtual bool print() = 0;
        [[nodiscard]] virtual bool email() = 0;
        // [[nodiscard]] virtual bool clear() = 0;
        [[nodiscard]] virtual bool save() = 0;
};
}
#endif
