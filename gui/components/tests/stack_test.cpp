/*******************************************************************************
 * Contents: stack unit tests
 * Author: Dawid Blom
 * Date: June 26, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"



#include <gtkmm.h>
#include <stack.h>
#include <iostream>
extern "C"
{

}


static void callback_function(const std::string& _stack_page_name)
{
	std::cout << "Stack page name: " << _stack_page_name << std::endl;
}

/**********************************GUI PART STACK TEST LIST***************
 * 1)
 ******************************************************************************/
TEST_GROUP(stack_test)
{
        gui::part::stack stack{"business-stack"};
        Glib::RefPtr<Gtk::Builder> builder;
        Glib::RefPtr<Gtk::Application> app;
	void setup()
	{
                app = Gtk::Application::create("org.testing");
                builder = Gtk::Builder::create();
                builder->add_from_file("../gui/admin-system.ui");
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

	CHECK_EQUAL(false, stack.subscribe("business-page", nullptr));
}

TEST(stack_test, subscribe_successfully)
{
	(void) stack.create(builder);

	CHECK_EQUAL(true, stack.subscribe("business-page", callback_function));
}

/*TEST(stack_test, get_current_stack_page_name_unsuccessfully)*/
/*{*/
/*	CHECK_EQUAL("", stack.current_stack_page());*/
/*}*/
/**/
/*TEST(stack_test, get_current_stack_page_name_successfully)*/
/*{*/
/*	(void) stack.create(builder);*/
/**/
/*	CHECK_EQUAL("invoice-page", stack.current_stack_page());*/
/*}*/
