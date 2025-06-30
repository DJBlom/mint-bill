
#ifndef _PART_H_
#define _PART_H_
#include <any>
#include <vector>
#include <string>
#include <gtkmm.h>
#include <functional>
#include <interfaces.h>



namespace interface {
class column_item {
public:
	virtual ~column_item() = default;

	[[nodiscard]] virtual bool create(const std::string&) = 0;
	[[nodiscard]] virtual bool is_not_valid() const = 0;
	[[nodiscard]] virtual Glib::RefPtr<Gtk::ColumnViewColumn> retrieve_item() const = 0;
	[[nodiscard]] virtual std::string retrieve_value() const = 0;
};

class column_view {
public:
	virtual ~column_view() = default;

	[[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
	[[nodiscard]] virtual bool is_not_valid() const = 0;
	[[nodiscard]] virtual bool add_column(const interface::column_item&) = 0;
	[[nodiscard]] virtual bool populate(const std::vector<std::any>&) = 0;
	[[nodiscard]] virtual bool clear() = 0;
	[[nodiscard]] virtual std::vector<std::any> extract() = 0;
};

class list_view {
public:
	virtual ~list_view() = default;

	[[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
	[[nodiscard]] virtual bool is_not_valid() const = 0;
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

class stack {
public:
	virtual ~stack() = default;

	[[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
	[[nodiscard]] virtual bool is_not_valid() const = 0;
	[[nodiscard]] virtual std::string current_stack_page() const = 0;
};

class test_button {
public:
	virtual ~test_button() = default;

	[[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&,
				   const interface::stack&) = 0;
	[[nodiscard]] virtual bool is_not_valid() const = 0;
	[[nodiscard]] virtual bool subscribe(const std::string&,
				      std::function<void(void)>) const = 0;
};

class search {
public:
	virtual ~search() = default;

	[[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&, const interface::stack&) = 0;
	[[nodiscard]] virtual bool is_not_valid() const = 0;
	[[nodiscard]] virtual bool subscribe(const std::string& page_name,
				      std::function<void(const std::string&)> callback) const = 0;
};


class observer {
public:
	virtual ~observer() = default;

	[[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
	[[nodiscard]] virtual bool is_not_valid() const = 0;
	[[nodiscard]] virtual bool subscribe(const std::string&,
				      std::function<void(const std::string&)>) const = 0;
};

class publisher {
public:
	virtual ~publisher() = default;

	[[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
	[[nodiscard]] virtual bool update(const std::string&) = 0;
	[[nodiscard]] virtual bool is_not_valid() const = 0;
	[[nodiscard]] virtual bool subscribe(const std::string&,
				      std::function<void(const std::string&)>) const = 0;
};
}
#endif
