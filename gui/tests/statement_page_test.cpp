/*******************************************************************************
 * Contents: statement page unit tests
 * Author: Dawid Blom
 * Date: May 27, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <gtkmm.h>
#include <stack.h>
#include <statement_page.h>
extern "C"
{

}


/***************************GUI PAGE STATEMENT TEST LIST***********************
 * 1) The statements data should be placed in columns on a single row. (Done)
 * 2) The user should be able to see the invoices associated with each
 *    statement. (Done)
 ******************************************************************************/
TEST_GROUP(statement_page_test)
{
        gui::statement_page page;
	std::shared_ptr<Gtk::Window> window{nullptr};
        Glib::RefPtr<Gtk::Builder> builder{nullptr};
        Glib::RefPtr<Gtk::Application> app{nullptr};
	gui::part::stack stack{"business-stack"};
        gui::part::search_bar search_bar{"business-name-search"};
	void setup()
	{
                app = Gtk::Application::create("org.testing");
                builder = Gtk::Builder::create();
                builder->add_from_file("../gui/mint-bill.ui");
		(void) search_bar.create(builder);
		window = std::make_shared<Gtk::Window>();
	}

	void teardown()
	{
	}
};

TEST(statement_page_test, create_statement_page_success)
{
        CHECK_EQUAL(true, page.create(builder, window));
}

TEST(statement_page_test, create_statement_page_fail_builder)
{
        Glib::RefPtr<Gtk::Builder> bad_builder{nullptr};

        CHECK_EQUAL(false, page.create(bad_builder, window));
}

TEST(statement_page_test, create_statement_page_fail_window)
{
	std::shared_ptr<Gtk::Window> bad_window{nullptr};

        CHECK_EQUAL(false, page.create(builder, bad_window));
}

TEST(statement_page_test, search_for_statements_success)
{
        (void) page.create(builder, window);

        CHECK_EQUAL(true, page.search("test"));
}

TEST(statement_page_test, search_for_statements_fail)
{
        (void) page.create(builder, window);

        CHECK_EQUAL(false, page.search(""));
}

TEST(statement_page_test, print_statements_success)
{
        (void) page.create(builder, window);

        CHECK_EQUAL(false, page.print());
}

TEST(statement_page_test, email_statements_success)
{
        (void) page.create(builder, window);

        CHECK_EQUAL(false, page.email());
}

TEST(statement_page_test, save_statements_success)
{
        (void) page.create(builder, window);

        CHECK_EQUAL(false, page.save());
}
