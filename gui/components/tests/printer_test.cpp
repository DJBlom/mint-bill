/*******************************************************************************
 * Contents: printer unit tests
 * Author: Dawid Blom
 * Date: July 10, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"



#include <gtkmm.h>
#include <iostream>
#include <printer.h>
#include <statement_page.h>
#include <client_statement.h>
#include <pdf_statement_data.h>
extern "C"
{

}



/********************************GUI PART PRINTER TEST LIST*********************
 * 1)
 ******************************************************************************/
TEST_GROUP(printer_test)
{
	gui::statement_page statement_page{};
        Glib::RefPtr<Gtk::Builder> builder;
        Glib::RefPtr<Gtk::Application> app;
	void setup()
	{
                app = Gtk::Application::create("org.testing");
                builder = Gtk::Builder::create();
                builder->add_from_file("../gui/mint-bill.ui");
		(void) statement_page.create(builder);
	}

	void teardown()
	{
                app.reset();
	}
};

TEST(printer_test, printer_is_not_connected)
{
        gui::part::printer printer{"statement"};

	CHECK_EQUAL(false, printer.is_connected());
}

TEST(printer_test, fail_to_print_all_documents)
{
	std::vector<std::any> vec{};
        gui::part::printer printer{"statement"};

	CHECK_EQUAL(false, printer.print(statement_page, vec));
}

TEST(printer_test, print_all_documents)
{
        feature::client_statement client_statement{};
	std::vector<std::any> statements{};
	for (const std::any& data : client_statement.load("Test Business Name"))
	{
		data::pdf_statement pdf_statement{std::any_cast<data::pdf_statement>(data)};
		statements.emplace_back(pdf_statement);
	}
        gui::part::printer printer{"statement"};

	CHECK_EQUAL(false, printer.print(statement_page, statements));
}
