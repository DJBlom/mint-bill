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
