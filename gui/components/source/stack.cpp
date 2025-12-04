/*******************************************************************************
 * @file    stack.cpp
 *
 * @brief   Implementation of the gui::part::stack class responsible for
 *          observing Gtk::Stack page changes and notifying subscribed GUI
 *          components.
 *
 * @details
 * The implementation performs the following:
 *
 *   - Retrieves a Gtk::Stack instance using the provided stack name.
 *   - Connects to the “visible-child-name” property change signal.
 *   - Updates internal state when the user or application switches pages.
 *   - Invokes all registered subscriber callbacks with the new page name.
 *
 * Key Behaviors:
 *   - create(): Initializes Gtk::Stack and attaches signal handlers.
 *   - subscribe(): Registers component-specific callbacks; updates callbacks
 *                  if the component re-subscribes.
 *   - set_current_page(): Allows programmatic UI navigation.
 *   - on_page_switched(): Central signal handler dispatching notifications.
 *
 * Logging:
 *   - Syslog is used to report invalid states, null pointers, or missing
 *     callbacks, ensuring predictable debugging in production builds.
 *
 * This implementation supports a clean observer pattern for GUI navigation,
 * enabling interaction between multiple GUI components without tight coupling.
 *******************************************************************************/
#include <stack.h>


gui::part::stack::stack(const std::string& _stack_name) : stack_name{_stack_name}
{
	this->stack_name.shrink_to_fit();
}

gui::part::stack::~stack() {}

bool gui::part::stack::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool success{false};
	if (!_ui_builder)
	{
                syslog(LOG_CRIT, "The UI builder is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		this->gui_stack = std::unique_ptr<Gtk::Stack>{
			_ui_builder->get_widget<Gtk::Stack>(this->stack_name)};
		if (this->is_not_valid())
		{
			syslog(LOG_CRIT, "The UI builder is not valid - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = true;
			this->current_page_name = this->gui_stack->get_visible_child_name();
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

bool gui::part::stack::subscribe(const std::string& _component,
				 std::function<void(const std::string&)> _callback) const
{
	bool success{false};
	if (_component.empty() || !_callback)
	{
		syslog(LOG_CRIT, "The _component or _callback is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		std::pair<
		std::unordered_map<
		std::string,
		std::function<void(const std::string&)>>::iterator,
		bool> result{this->subscribers.emplace(_component, _callback)};
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

std::string gui::part::stack::current_page() const
{
	return this->current_page_name;
}

bool gui::part::stack::set_current_page(const std::string& _stack_page_name) const
{
	bool success{false};
	if (_stack_page_name.empty() == true)
	{
		syslog(LOG_CRIT, "STACK: invalid argumnet - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->gui_stack->set_visible_child(_stack_page_name);
	}

	return success;
}

void gui::part::stack::on_page_switched()
{
	if (this->is_not_valid())
	{
		syslog(LOG_CRIT, "Stack is null during page switch");
	}
	else
	{
		this->current_page_name = this->gui_stack->get_visible_child_name();
		for (const auto& [component, callback] : this->subscribers)
		{
			if (!callback)
			{
				syslog(LOG_CRIT, "The subscriber has not been registered - "
						  "filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				callback(this->current_page_name);
			}
		}
	}
}
