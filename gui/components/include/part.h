/*******************************************************************************
 * @file part.h
 *
 * @brief Abstract GUI interfaces for composable view, dialog, and event
 *        components.
 *
 * @details
 * This header defines a small set of pure-virtual interfaces used throughout
 * the GUI layer to decouple high-level pages from concrete Gtkmm widgets.
 * Implementations in gui_parts.h / gui_parts.cpp and other modules provide
 * concrete behavior while callers interact only with these interfaces.
 *
 * The interfaces are:
 *
 *   - interface::column_item
 *       * Represents a single column in a Gtk::ColumnView.
 *       * Responsible for:
 *           - create(): building the underlying Gtk::ColumnViewColumn and its
 *             cell factory using a column title.
 *           - is_not_valid(): runtime validity check of internal state.
 *           - retrieve_item(): returning the configured ColumnViewColumn.
 *           - retrieve_value(): returning the last bound cell value (useful
 *             for testing, debugging, or exporting).
 *
 *   - interface::column_view
 *       * Encapsulates a Gtk::ColumnView and its backing model.
 *       * Responsibilities:
 *           - create(): resolve the underlying widget from Gtk::Builder and
 *             initialize its model.
 *           - add_column(): append a column_item to the view.
 *           - populate(): load a list of records (std::any) into the model.
 *           - clear(): remove all items from the model.
 *           - extract(): return the current records as a vector<std::any>.
 *
 *   - interface::list_view
 *       * Generic list view abstraction (e.g., Gtk::ListView without
 *         callbacks). It exposes the same basic lifecycle as column_view but
 *         for single-column or simpler list presentations.
 *
 *   - interface::list_view_callback
 *       * Extends list_view with user interaction callbacks:
 *           - double_click(): register a handler for double-click/activate
 *             actions on a single item.
 *           - single_click(): register a handler that receives the currently
 *             selected set of items (e.g., for multi-selection cases).
 *
 *   - interface::dialog
 *       * Abstract message/confirmation dialog:
 *           - create(): resolve the dialog from Gtk::Builder and connect its
 *             response signal.
 *           - connect(): register a response callback (receives int response
 *             codes such as GTK_RESPONSE_YES / NO / CLOSE).
 *           - show()/hide(): control visibility with validation hooks.
 *
 *   - interface::observer
 *       * Generic subscription target in an observer pattern:
 *           - subscribe(): register a per-page callback identified by a
 *             string key; callbacks receive a std::string payload (commonly
 *             a search keyword or similar).
 *
 *   - interface::publisher
 *       * Complement to observer, representing a source of notifications:
 *           - update(): notify subscribers about a context change (e.g.,
 *             active stack page).
 *           - subscribe(): register subscribers tied to logical page names.
 *
 *   - interface::dispatcher
 *       * Simple event dispatcher abstraction, typically wrapping a single
 *         widget (e.g., a button):
 *           - subscribe(): attach a void() callback triggered by the GUI
 *             event (e.g., button clicked).
 *
 * By relying on these interfaces instead of concrete Gtkmm types in higher-
 * level code, the GUI remains easier to test, mock, and evolve. Pages like
 * invoice_page or statement_page can depend only on interface::* types,
 * while actual Gtk-based implementations live in gui::part and related
 * namespaces.
 *******************************************************************************/
#ifndef _PART_H_
#define _PART_H_
#include <any>
#include <vector>
#include <string>
#include <gtkmm.h>
#include <functional>



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

class list_view_callback {
public:
	virtual ~list_view_callback() = default;

	[[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
	[[nodiscard]] virtual bool is_not_valid() const = 0;
	[[nodiscard]] virtual bool populate(const std::vector<std::any>&) = 0;
	[[nodiscard]] virtual bool clear() = 0;
	[[nodiscard]] virtual std::vector<std::any> extract() = 0;
	[[nodiscard]] virtual bool double_click(std::function<void(const std::any&)>) = 0;
	[[nodiscard]] virtual bool single_click(std::function<void(const std::vector<std::any>&)>) = 0;
};

class dialog {
public:
	virtual ~dialog() = default;

	[[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
	[[nodiscard]] virtual bool is_not_valid() const = 0;
	[[nodiscard]] virtual bool connect(const std::function<void(const int&)>&) = 0;
	[[nodiscard]] virtual bool show() const = 0;
	[[nodiscard]] virtual bool hide() const = 0;
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

class dispatcher {
public:
	virtual ~dispatcher() = default;

	[[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) = 0;
	[[nodiscard]] virtual bool is_not_valid() const = 0;
	[[nodiscard]] virtual bool subscribe(std::function<void(void)>) = 0;
};
}
#endif
