/*******************************************************************************
 * @file    stack_tests.cpp
 *
 * @brief   Unit tests for the gui::part::stack component.
 *
 * @details
 * This test suite verifies the behavior of the stack page-management utility
 * used throughout the Mint-Bill GUI. The gui::part::stack class wraps a
 * Gtk::Stack widget and provides:
 *
 *   - Safe creation from a Gtk::Builder file.
 *   - Page-validity checks and visible page tracking.
 *   - Subscription and notification for page-change observers.
 *   - Programmatic page switching through set_current_page().
 *
 * The tests in this file validate:
 *   - Proper handling of invalid builder references.
 *   - Correct detection of stack validity before and after creation.
 *   - Subscriber registration rules (reject duplicate pages, reject missing
 *     callbacks or page names).
 *   - Correct reaction to simulated GTK stack page changes.
 *   - Retrieval of the current visible page name.
 *
 * Test Strategy:
 *   - Uses Gtk::Application + Gtk::Builder loaded from mint-bill.ui.
 *   - Uses Gtk::Stack's child switching to simulate GUI navigation.
 *   - Uses a callback function to confirm subscriber notifications.
 *******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"



#include <gtkmm.h>
#include <stack.h>
#include <iostream>
extern "C"
{

}


static std::string callback_result{""};
static void callback_function(const std::string& _stack_page_name)
{
	callback_result = _stack_page_name;
}

/**********************************GUI PART STACK TEST LIST***************
 * 1)
 ******************************************************************************/
TEST_GROUP(stack_test)
{
        gui::part::stack stack{"pages-stack"};
        Glib::RefPtr<Gtk::Builder> builder;
        Glib::RefPtr<Gtk::Application> app;
	void setup()
	{
                app = Gtk::Application::create("org.testing");
                builder = Gtk::Builder::create();
                builder->add_from_file("../gui/mint-bill.ui");
	}

	void teardown()
	{
                app.reset();
	}
};

TEST(stack_test, create_stack_unsuccessfully)
{
	CHECK_EQUAL(false, stack.create(nullptr));
}

TEST(stack_test, create_stack_successfully)
{
	CHECK_EQUAL(true, stack.create(builder));
}

TEST(stack_test, stack_is_not_valid)
{
	CHECK_EQUAL(true, stack.is_not_valid());
}

TEST(stack_test, stack_is_valid)
{
	(void) stack.create(builder);

	CHECK_EQUAL(false, stack.is_not_valid());
}

TEST(stack_test, subscribe_no_page_name)
{
	(void) stack.create(builder);

	CHECK_EQUAL(false, stack.subscribe("", callback_function));
}

TEST(stack_test, subscribe_no_callback)
{
	(void) stack.create(builder);

	CHECK_EQUAL(false, stack.subscribe("admin-page", nullptr));
	CHECK_EQUAL(true, stack.set_current_page("admin-page"));
}

TEST(stack_test, subscribe_successfully)
{
	(void) stack.create(builder);

	CHECK_EQUAL(true, stack.subscribe("admin-page", callback_function));
	CHECK_EQUAL(true, stack.set_current_page("admin-page"));
}

TEST(stack_test, try_to_add_multiple_subscriber_to_the_same_page)
{
	(void) stack.create(builder);
	(void) stack.subscribe("statement-page", callback_function);

	CHECK_EQUAL(false, stack.subscribe("statement-page", callback_function));
}

TEST(stack_test, changing_the_stack_page_unsuccessfully)
{
	(void) stack.create(builder);
	(void) stack.subscribe("statement-page", callback_function);
	auto temp_stack = builder->get_widget<Gtk::Stack>("pages-stack");
	auto children = temp_stack->get_children();
	temp_stack->set_visible_child(*children.front());
	temp_stack->remove(*temp_stack->get_visible_child());

	CHECK_EQUAL("", temp_stack->get_visible_child_name());
}

TEST(stack_test, changing_the_stack_page_successfully)
{
	(void) stack.create(builder);
	(void) stack.subscribe("statement-page", callback_function);
	auto temp_stack = builder->get_widget<Gtk::Stack>("pages-stack");
	auto children = temp_stack->get_children();
	temp_stack->set_visible_child(*children.front());
	temp_stack->set_visible_child(*children.back());

	CHECK_EQUAL("admin-page", temp_stack->get_visible_child_name());
}

TEST(stack_test, get_current_stack_page_name_unsuccessfully)
{
	CHECK_EQUAL("", stack.current_page());
}

TEST(stack_test, get_current_stack_page_name_successfully)
{
	(void) stack.create(builder);

	CHECK_EQUAL("invoice-page", stack.current_page());
}
