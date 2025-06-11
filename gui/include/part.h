
#ifndef _PART_H_
#define _PART_H_
#include <gtkmm.h>
#include <interfaces.h>

namespace interface {
//class view {
//public:
//        virtual ~view() = default;
//
//        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
//        [[nodiscard]] virtual bool connect(const interface::dialog&) = 0;
//};


class item {
public:
        virtual ~item() = default;

        [[nodiscard]] virtual bool is_not_valid() const = 0;
        [[nodiscard]] virtual Glib::RefPtr<Gtk::ColumnViewColumn> retrieve_item() const = 0;
        [[nodiscard]] virtual std::string retrieve_value() const = 0;
};

class view {
public:
        virtual ~view() = default;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
        [[nodiscard]] virtual bool is_not_valid() const = 0;
        [[nodiscard]] virtual bool add_column(const interface::item&) = 0;
        [[nodiscard]] virtual bool populate(const interface::feature&) = 0;
};

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
