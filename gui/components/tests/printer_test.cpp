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
#include <statement_pdf.h>
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
        feature::client_statement client_statement{};
	std::vector<std::shared_ptr<poppler::document>> statements{};
	feature::statement_pdf statement_pdf{};
	gui::statement_page statement_page{};
        Glib::RefPtr<Gtk::Builder> builder;
        Glib::RefPtr<Gtk::Application> app;
	void setup()
	{
                app = Gtk::Application::create("org.testing");
                builder = Gtk::Builder::create();
                builder->add_from_file("../gui/mint-bill.ui");
		(void) statement_page.create(builder);
		for (const std::any& data : client_statement.load("Test Business Name"))
		{
			data::pdf_statement pdf_statement{std::any_cast<data::pdf_statement>(data)};
			statements.emplace_back(statement_pdf.generate_for_print(pdf_statement));
		}
	}

	void teardown()
	{
                app.reset();
	}
};

TEST(printer_test, printer_is_not_connected)
{
	std::vector<std::shared_ptr<poppler::document>> vec{};
        gui::part::printer printer{"statement", vec};

	CHECK_EQUAL(false, printer.is_connected());
}

TEST(printer_test, fail_to_print_all_documents)
{
	std::vector<std::shared_ptr<poppler::document>> vec{};
        gui::part::printer printer{"statement", vec};

	CHECK_EQUAL(false, printer.print(statement_page));
}

TEST(printer_test, print_all_documents)
{
        gui::part::printer printer{"statement", statements};

	CHECK_EQUAL(true, printer.print(statement_page));
}




/****************************GUI PRINT PAGE RANGE TEST LIST*********************
 * 1) Check if the given page is in range.
 ******************************************************************************/
TEST_GROUP(page_range_test)
{
	int first_page{1};
	int number_of_pages{10};
	int document_index{1};
	gui::part::page_range page_range{first_page, number_of_pages, document_index};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(page_range_test,  check_if_the_given_page_number_is_not_range)
{
	int current_page_number{11};

	CHECK_EQUAL(false, page_range.check(current_page_number));
}

TEST(page_range_test,  check_if_the_given_page_number_is_in_range)
{
	int current_page_number{1};

	CHECK_EQUAL(true, page_range.check(current_page_number));
}

TEST(page_range_test,  retrieve_the_local_page)
{
	int current_page_number{1};

	CHECK_EQUAL(0, page_range.local_page(current_page_number));
}

TEST(page_range_test,  retrieve_the_current_document)
{
	CHECK_EQUAL(1, page_range.current_document());
}
