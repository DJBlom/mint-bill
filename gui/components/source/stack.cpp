#include <stack.h>
#include <iostream>

gui::part::stack::stack(const std::string& _stack_name) : stack_name{_stack_name}
{
	this->stack_name.shrink_to_fit();
}

gui::part::stack::~stack() {}

bool gui::part::stack::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{true};
	if (!_ui_builder)
	{
		success = false;
	}
	else
	{
		this->gui_stack = std::unique_ptr<Gtk::Stack>{
			_ui_builder->get_widget<Gtk::Stack>(this->stack_name)};
		if (!this->gui_stack)
		{
			success = false;
		}
		else
		{
			this->gui_stack->property_visible_child_name()
				.signal_changed()
				.connect(sigc::mem_fun(*this, &stack::on_page_switched));
		}
	}

	return success;
}

bool gui::part::stack::is_not_valid() const
{
	return !(this->gui_stack);
}

bool gui::part::stack::subscribe(const std::string& _page_name,
				 std::function<void(const std::string&)> _callback) const
{
	bool success{true};
	if (_page_name.empty() || !_callback)
	{
		success = false;
	}
	else
	{
		std::pair<
		std::unordered_map<
		std::string,
		std::function<void(const std::string&)>>::iterator,
		bool> result{this->subscribers.emplace(_page_name, _callback)};
		if (result.second)
		{
			success = true;
		}
		else
		{
			result.first->second = _callback;
		}
	}

	return success;
}

void gui::part::stack::on_page_switched()
{
	std::unordered_map<
		std::string,
		std::function<void(const std::string&)>
		>::iterator iterator = this->subscribers.find(this->gui_stack->get_visible_child_name());
	if (iterator == this->subscribers.end())
	{
		syslog(LOG_CRIT, "The subscriber has not been registered - "
			 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		iterator->second(this->gui_stack->get_visible_child_name());
	}
}
