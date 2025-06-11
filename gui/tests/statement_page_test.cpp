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
#include <statement_page.h>
extern "C"
{

}


/***************************GUI PAGE STATEMENT TEST LIST***********************
 * 1) The statements data should be placed in columns on a single row.
 * 2) The user should be able to see the invoices associated with each
 *    statement.
 * 3)
 ******************************************************************************/
TEST_GROUP(statement_page_test)
{
        gui::statement_page page;
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
	}
};

TEST(statement_page_test, create_statement_page)
{
        CHECK_EQUAL(true, page.create(builder));
}
