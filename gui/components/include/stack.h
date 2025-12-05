/*******************************************************************************
 * @file    stack.h
 *
 * @brief   Declaration of the gui::part::stack class, an observer-based wrapper
 *          around Gtk::Stack for page-change monitoring and event dispatching.
 *
 * @details
 * The stack class implements the interface::observer interface and provides a
 * high-level GUI component that:
 *
 *   - Wraps a Gtk::Stack retrieved from a Gtk::Builder UI definition.
 *   - Tracks and exposes the currently visible page.
 *   - Emits page-change notifications to registered subscribers.
 *   - Allows programmatic page switching by page name.
 *
 * Responsibilities:
 *   - Register subscribers keyed by component name.
 *   - Store callbacks and invoke them whenever the visible child changes.
 *   - Provide a simple "is_not_valid()" state check.
 *   - Maintain the current page name for external query.
 *
 * Error Handling:
 *   - Logs syslog errors for invalid UI objects, empty names, and missing
 *     callbacks.
 *
 * This class is non-copyable and non-movable to prevent state divergence
 * caused by duplicated subscriber maps or widget ownership.
 *******************************************************************************/
#ifndef _STACK_H_
#define _STACK_H_
#include <part.h>
#include <syslog.h>
#include <unordered_map>

namespace gui {
namespace part {
class stack : public interface::observer {
public:
	stack() = delete;
	explicit stack(const std::string&);
	stack(const stack&) = delete;
	stack(stack&&) = delete;
	stack& operator= (const stack&) = delete;
	stack& operator= (stack&&) = delete;
	~stack() override;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;
        [[nodiscard]] virtual bool is_not_valid() const override;
	[[nodiscard]] virtual bool subscribe(const std::string&,
				      std::function<void(const std::string&)>) const override;
	[[nodiscard]] virtual std::string current_page() const;
	[[nodiscard]] virtual bool set_current_page(const std::string&) const;

private:
	void on_page_switched();

private:
	std::string stack_name{""};
	std::string current_page_name{""};
        std::unique_ptr<Gtk::Stack> gui_stack{};
	mutable std::unordered_map<std::string,
		std::function<void(const std::string&)>> subscribers{};
};
}
}
#endif
