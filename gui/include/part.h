
#ifndef _PART_H_
#define _PART_H_
#include <any>
#include <vector>
#include <string>
#include <gtkmm.h>
#include <functional>
#include <interfaces.h>



namespace interface {
class search {
public:
        virtual ~search() = default;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
        [[nodiscard]] virtual bool is_not_valid() const = 0;
	[[nodiscard]] virtual bool subscribe(const std::string& page_name,
				      std::function<void(const std::string&)> callback) const = 0;
};

class item {
public:
        virtual ~item() = default;

        [[nodiscard]] virtual bool create(const std::string&) = 0;
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
        [[nodiscard]] virtual bool populate(const std::vector<std::any>&) = 0;
        [[nodiscard]] virtual bool clear() = 0;
        [[nodiscard]] virtual std::vector<std::any> extract() = 0;
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
