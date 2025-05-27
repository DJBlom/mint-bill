
#ifndef _PART_H_
#define _PART_H_
#include <gtkmm.h>
namespace interface {
class dialog {
public:
        virtual ~dialog() = default;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
        [[nodiscard]] virtual bool is_not_valid() const = 0;
        [[nodiscard]] virtual bool connect() const = 0;
        virtual void show() const = 0;
};

class button {
public:
        virtual ~button() = default;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
        [[nodiscard]] virtual bool connect(const interface::dialog&) = 0;
        virtual void disable() = 0;
        virtual void enable() = 0;
};
}
#endif
