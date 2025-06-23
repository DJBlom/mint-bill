
/*******************************************************************************
 * Contents: gui parts unit tests
 * Author: Dawid Blom
 * Date: May 27, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"



#include <gtkmm.h>
#include <iostream>
#include <statement_page.h>
#include <statement_data.h>
#include <client_statement.h>
#include <mock_pdf_invoice_data.h>
extern "C"
{

}


static std::string callback_result{""};
static void callback_function(const std::string& keyword) {
    callback_result = keyword;
}


/**********************************GUI PART SEARCH BAR TEST LIST***************
 * 1) The user should be able to search for a business. Done
 * 2) There should be no elements in the list if the user clears the
 *    search. Done
 * 3) When the user navigates to a different page, the search entry should
 *    be automatically cleared. Done
 ******************************************************************************/
TEST_GROUP(gui_part_search_bar)
{
        gui::part::search_bar search_bar{"business-stack", "business-name-search"};
        Glib::RefPtr<Gtk::Builder> builder;
        Glib::RefPtr<Gtk::Application> app;
	void setup()
	{
                app = Gtk::Application::create("org.testing");
                builder = Gtk::Builder::create();
                builder->add_from_file("../gui/admin-system.ui");
		callback_result.clear();
	}

	void teardown()
	{
                app.reset();
	}
};

TEST(gui_part_search_bar, create_search_bar_successfully)
{
	CHECK_EQUAL(true, search_bar.create(builder));
}

TEST(gui_part_search_bar, create_search_bar_unsuccessfully)
{
	CHECK_EQUAL(false, search_bar.create(nullptr));
}

TEST(gui_part_search_bar, is_search_bar_not_valid)
{
	CHECK_EQUAL(true, search_bar.is_not_valid());
}

TEST(gui_part_search_bar, is_search_bar_valid)
{
	(void) search_bar.create(builder);

	CHECK_EQUAL(false, search_bar.is_not_valid());
}

TEST(gui_part_search_bar, fail_to_add_subscriber_without_page_name)
{
	(void) search_bar.create(builder);

	CHECK_EQUAL(false, search_bar.subscribe("", callback_function));
}

TEST(gui_part_search_bar, fail_to_add_subscriber_without_callback_function)
{
	(void) search_bar.create(builder);

	CHECK_EQUAL(false, search_bar.subscribe("statement-page", nullptr));
}

TEST(gui_part_search_bar, add_subscriber)
{
	(void) search_bar.create(builder);

	CHECK_EQUAL(true, search_bar.subscribe("statement-page", callback_function));
}

TEST(gui_part_search_bar, try_to_add_multiple_subscriber_to_the_same_page)
{
	(void) search_bar.create(builder);
	(void) search_bar.subscribe("statement-page", callback_function);

	CHECK_EQUAL(false, search_bar.subscribe("statement-page", callback_function));
}

TEST(gui_part_search_bar, clear_the_search_bar_entry_on_every_stack_page_change)
{
	(void) search_bar.create(builder);
	(void) search_bar.subscribe("statement-page", callback_function);
	auto stack = builder->get_widget<Gtk::Stack>("business-stack");
	auto entry = builder->get_widget<Gtk::SearchEntry>("business-name-search");
	entry->set_text("invoice123");
	stack->set_visible_child("statement-page", Gtk::StackTransitionType::NONE);
	g_signal_emit_by_name(entry->gobj(), "search-changed");

	CHECK_EQUAL("", entry->get_text());
}

TEST(gui_part_search_bar, changing_the_search_keyword_unsuccessfully)
{
	(void) search_bar.create(builder);
	(void) search_bar.subscribe("statement-page", callback_function);
	auto entry = builder->get_widget<Gtk::SearchEntry>("business-name-search");
	entry->set_text("");
	g_signal_emit_by_name(entry->gobj(), "search-changed");

	CHECK_EQUAL("", entry->get_text());
}

TEST(gui_part_search_bar, changing_the_search_keyword_successfully)
{
	std::string result{"Test Business"};
	(void) search_bar.create(builder);
	(void) search_bar.subscribe("statement-page", callback_function);
	auto entry = builder->get_widget<Gtk::SearchEntry>("business-name-search");
	entry->set_text(result);
	g_signal_emit_by_name(entry->gobj(), "search-changed");
	std::string expected{entry->get_text()};

	STRCMP_EQUAL(result.c_str(), expected.c_str());
}

TEST(gui_part_search_bar, reach_the_end_of_subscriber_list)
{
	std::string expected{"Test Business"};
	(void) search_bar.create(builder);
	(void) search_bar.subscribe("statement-page", callback_function);
	auto stack = builder->get_widget<Gtk::Stack>("business-stack");
	auto entry = builder->get_widget<Gtk::SearchEntry>("business-name-search");
	stack->set_visible_child("statement-page", Gtk::StackTransitionType::NONE);
	entry->set_text(expected);
	g_signal_emit_by_name(entry->gobj(), "search-changed");

	STRCMP_EQUAL(expected.c_str(), callback_result.c_str());
}




/**********************************GUI PART DIALOG TEST LIST*******************
 * 1) create the dialog. (Done)
 * 2) connect the signal handler of the dialog. (Done)
 * 3) show the dialog. (Done)
 * 4) ensure that the system does not crash if we connect the dialog
 *    before it was created. (Done)
 * 5) verify that the dialog is valid before using it. (Done)
 ******************************************************************************/
TEST_GROUP(gui_part_dialog)
{
        gui::part::dialog gui_dialog{"statement-save-button-alert"};
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

TEST(gui_part_dialog, dialog_create)
{
        CHECK_EQUAL(true, gui_dialog.create(builder));
}

TEST(gui_part_dialog, dialog_is_valid)
{
        (void) gui_dialog.create(builder);

        CHECK_EQUAL(false, gui_dialog.is_not_valid());
}

TEST(gui_part_dialog, dialog_connect_successfully)
{
        (void) gui_dialog.create(builder);

        CHECK_EQUAL(true, gui_dialog.connect());
}

TEST(gui_part_dialog, dialog_connect_unsuccessfully)
{
        CHECK_EQUAL(false, gui_dialog.connect());
}

TEST(gui_part_dialog, dialog_show)
{
        CHECK_EQUAL(true, gui_dialog.create(builder));
        CHECK_EQUAL(true, gui_dialog.connect());
        gui_dialog.show();
}







/**********************************GUI PART BUTTON TEST LIST*******************
 * 1) create the button. (Done)
 * 2) connect the button. (Done)
 * 3) disable and enable the button. (Done)
 ******************************************************************************/
TEST_GROUP(gui_part_button)
{
        gui::part::button gui_button{"statement-save-button"};
        gui::part::dialog dialog{"statement-save-button-alert"};
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

TEST(gui_part_button, button_create)
{
        CHECK_EQUAL(true, gui_button.create(builder));
}

TEST(gui_part_button, button_connect)
{
        CHECK_EQUAL(true, dialog.create(builder));
        CHECK_EQUAL(true, gui_button.create(builder));
        CHECK_EQUAL(true, gui_button.connect(dialog));
}

TEST(gui_part_button, button_disable)
{
        CHECK_EQUAL(true, dialog.create(builder));
        CHECK_EQUAL(true, gui_button.create(builder));
        CHECK_EQUAL(true, gui_button.connect(dialog));
        gui_button.disable();
}

TEST(gui_part_button, button_enable)
{
        CHECK_EQUAL(true, dialog.create(builder));
        CHECK_EQUAL(true, gui_button.create(builder));
        CHECK_EQUAL(true, gui_button.connect(dialog));
        gui_button.enable();
}








/**********************************GUI PART COLUMN_VIEW TEST LIST**************
 * 1) Create the column view. (Done)
 * 3) Extract data from the column view. (Done)
 * 4) Populate the column view. (Done)
 * 5) Clear the column view. (Done)
 * 6) Ensure the column view is valid. (Done)
 * 7) Add a column to the column view. (Done)
 * 8) Add a single data line to the column view. (Done)
 * 9) Populate the column view based on a search.
 ******************************************************************************/
TEST_GROUP(statement_page_column_view_test)
{
        Glib::RefPtr<Gtk::Builder> builder{};
        Glib::RefPtr<Gtk::Application> app{};
        gui::part::statement::column_view column_view{"statement-column-view", "statement-column-view-vadjustment"};
        gui::part::statement::columns::date date{};
        gui::part::statement::columns::price price{};
        gui::part::statement::columns::paid_status paid_status{};
        gui::part::statement::columns::order_number order_number{};
        gui::part::statement::columns::invoice_number invoice_number{};
	void setup()
	{
                app = Gtk::Application::create("org.testing");
                builder = Gtk::Builder::create();
                builder->add_from_file("../gui/admin-system.ui");
		(void) date.create("Date");
		(void) price.create("Price");
		(void) paid_status.create("Paid Status");
		(void) order_number.create("Order Number");
		(void) invoice_number.create("Invoice Number");
	}

	void teardown()
	{
                app.reset();
	}
};

TEST(statement_page_column_view_test, column_view_create)
{
        CHECK_EQUAL(true, column_view.create(builder));
}

TEST(statement_page_column_view_test, column_view_is_valid)
{
        (void) column_view.create(builder);

        CHECK_EQUAL(false, column_view.is_not_valid());
}

TEST(statement_page_column_view_test, column_view_add_column)
{
        (void) column_view.create(builder);
        (void) column_view.is_not_valid();

        CHECK_EQUAL(true, column_view.add_column(invoice_number));
        CHECK_EQUAL(true, column_view.add_column(date));
        CHECK_EQUAL(true, column_view.add_column(order_number));
        CHECK_EQUAL(true, column_view.add_column(paid_status));
        CHECK_EQUAL(true, column_view.add_column(price));
}

TEST(statement_page_column_view_test, column_view_populate)
{
        feature::client_statement client_statement{};
        (void) column_view.create(builder);
        (void) column_view.is_not_valid();
        (void) column_view.add_column(invoice_number);
        (void) column_view.add_column(date);
        (void) column_view.add_column(order_number);
        (void) column_view.add_column(paid_status);
        (void) column_view.add_column(price);

        CHECK_EQUAL(true, column_view.populate(client_statement.load("business")));
}

TEST(statement_page_column_view_test, column_view_clear_unsuccessful)
{
        feature::client_statement client_statement{};

        CHECK_EQUAL(false, column_view.clear());
}

TEST(statement_page_column_view_test, column_view_clear_successful)
{
        feature::client_statement client_statement{};
        (void) column_view.create(builder);
        (void) column_view.is_not_valid();
        (void) column_view.add_column(invoice_number);
        (void) column_view.add_column(date);
        (void) column_view.add_column(order_number);
        (void) column_view.add_column(paid_status);
        (void) column_view.add_column(price);
        (void) column_view.populate(client_statement.load("business"));

        CHECK_EQUAL(true, column_view.clear());
}

TEST(statement_page_column_view_test, extract_data_from_store)
{
        feature::client_statement client_statement{};
        (void) column_view.create(builder);
        (void) column_view.is_not_valid();
        (void) column_view.add_column(invoice_number);
        (void) column_view.add_column(date);
        (void) column_view.add_column(order_number);
        (void) column_view.add_column(paid_status);
        (void) column_view.add_column(price);
        (void) column_view.populate(client_statement.load("business"));
	std::vector<std::any> records(column_view.extract());

        CHECK_EQUAL(100, records.size());
}




/**********************************GUI PART LIST_VIEW TEST LIST**************
 * 1) Create the list view. (Done)
 * 3) Extract data from the list view.
 * 4) Populate the list view.
 * 5) Clear the list view.
 * 6) Ensure the list view is valid.
 * 7) Add a column to the list view.
 * 8) Add a single data line to the list view.
 * 9) Populate the list view based on a search.
 ******************************************************************************/
TEST_GROUP(invoice_pdf_view_test)
{
        Glib::RefPtr<Gtk::Builder> builder{};
        Glib::RefPtr<Gtk::Application> app{};
        gui::part::statement::invoice_pdf_view invoice_pdf_view{"statement-invoice-list-view", "statement-invoice-list-view-vadjustment"};
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

TEST(invoice_pdf_view_test, invoice_pdf_view_create_unsuccessfully)
{
        CHECK_EQUAL(false, invoice_pdf_view.create(nullptr));
}

TEST(invoice_pdf_view_test, create_successfully)
{
        CHECK_EQUAL(true, invoice_pdf_view.create(builder));
}

TEST(invoice_pdf_view_test, view_is_not_valid)
{
        CHECK_EQUAL(true, invoice_pdf_view.is_not_valid());
}

TEST(invoice_pdf_view_test, view_is_valid)
{
        (void) invoice_pdf_view.create(builder);

        CHECK_EQUAL(false, invoice_pdf_view.is_not_valid());
}

TEST(invoice_pdf_view_test, populate_view_without_being_created)
{
	std::vector<std::any> invoices{};

        CHECK_EQUAL(false, invoice_pdf_view.populate(invoices));
}

TEST(invoice_pdf_view_test, populate_view_with_empty_data)
{
	std::vector<std::any> invoices{};
        (void) invoice_pdf_view.create(builder);

        CHECK_EQUAL(false, invoice_pdf_view.populate(invoices));
}

TEST(invoice_pdf_view_test, populate_view_with_bad_pdf_invoice_data)
{
        data::pdf_invoice invoice_data;
	std::vector<std::any> invoices{};
	invoice_data.set_business(retrieve_bad_business_data());
	invoice_data.set_client(retrieve_bad_client_data());
	invoice_data.set_invoice(retrieve_bad_invoice_data());
	invoices.push_back(invoice_data);
        (void) invoice_pdf_view.create(builder);

        CHECK_EQUAL(false, invoice_pdf_view.populate(invoices));
}

TEST(invoice_pdf_view_test, populate_invoice_view_successfully)
{
        data::pdf_invoice invoice_data;
	std::vector<std::any> invoices{};
	invoice_data.set_business(retrieve_business_data());
	invoice_data.set_client(retrieve_client_data());
	invoice_data.set_invoice(retrieve_invoice_data());
	invoices.push_back(invoice_data);
        (void) invoice_pdf_view.create(builder);

        CHECK_EQUAL(true, invoice_pdf_view.populate(invoices));
}

TEST(invoice_pdf_view_test, clear_invoice_view_data_unsuccessfully)
{
        CHECK_EQUAL(false, invoice_pdf_view.clear());
}

TEST(invoice_pdf_view_test, clear_invoice_view_data_successfully)
{
        data::pdf_invoice invoice_data;
	std::vector<std::any> invoices{};
	invoice_data.set_business(retrieve_business_data());
	invoice_data.set_client(retrieve_client_data());
	invoice_data.set_invoice(retrieve_invoice_data());
	invoices.push_back(invoice_data);
        (void) invoice_pdf_view.create(builder);
        (void) invoice_pdf_view.populate(invoices);

        CHECK_EQUAL(true, invoice_pdf_view.clear());
}

TEST(invoice_pdf_view_test, extract_invoice_view_data_successfully)
{
        data::pdf_invoice invoice_data;
	std::vector<std::any> invoices{};
	invoice_data.set_business(retrieve_business_data());
	invoice_data.set_client(retrieve_client_data());
	invoice_data.set_invoice(retrieve_invoice_data());
	invoices.push_back(invoice_data);
        (void) invoice_pdf_view.create(builder);
        (void) invoice_pdf_view.populate(invoices);
	std::vector<std::any> expected{invoice_pdf_view.extract()};

        CHECK_EQUAL(false, expected.empty());
}

/*TEST(invoice_pdf_view_test, display_invoice)*/
/*{*/
/*        data::pdf_invoice invoice_data;*/
/*	std::vector<std::any> invoices{};*/
/*	invoice_data.set_business(retrieve_business_data());*/
/*	invoice_data.set_client(retrieve_client_data());*/
/*	invoice_data.set_invoice(retrieve_invoice_data());*/
/*	invoices.push_back(invoice_data);*/
/*        (void) invoice_pdf_view.create(builder);*/
/*        (void) invoice_pdf_view.populate(invoices);*/
/**/
/*        CHECK_EQUAL(true, expected.empty());*/
/*}*/



/*************************GUI PART LIST ITEM TEST LIST**************************
 * 1)
  ******************************************************************************/
/*TEST_GROUP(statement_list_item_test)*/
/*{*/
/*	data::invoice invoice{};*/
/*	Glib::RefPtr<Gtk::Builder> builder{};*/
/*	Glib::RefPtr<Gtk::Application> app{};*/
/*	std::unique_ptr<Gtk::ListView> view{};*/
/*	Glib::RefPtr<Gtk::SelectionModel> selection{};*/
/*	gui::part::statement::rows::list_information invoice_information{};*/
/*	Glib::RefPtr<Gio::ListStore<gui::part::statement::rows::entries>> store{};*/
/*	void setup()*/
/*	{*/
/*		app = Gtk::Application::create("org.testing");*/
/*		builder = Gtk::Builder::create();*/
/*		builder->add_from_file("../gui/admin-system.ui");*/
/*		view = std::unique_ptr<Gtk::ListView>{*/
/*			builder->get_widget<Gtk::ListView>("statement-invoice-list-view")};*/
/*		store = Gio::ListStore<gui::part::statement::rows::entries>::create();*/
/*		store->append(gui::part::statement::rows::entries::create(invoice));*/
/*		selection = Gtk::MultiSelection::create(store);*/
/*		view->set_model(selection);*/
/*	}*/
/**/
/*	void teardown()*/
/*	{*/
/*		app.reset();*/
/*	}*/
/*};*/
/**/
/*TEST(statement_list_item_test, create_list_item)*/
/*{*/
/*	CHECK_EQUAL(true, invoice_information.create());*/
/*}*/
/**/
/*TEST(statement_list_item_test, list_item_is_valid)*/
/*{*/
/*	CHECK_EQUAL(true, invoice_information.is_not_valid());*/
/*}*/
/**/
/*TEST(statement_list_item_test, list_item_retrieve_item)*/
/*{*/
/*	(void) invoice_information.create();*/
/*        Glib::RefPtr<Gtk::SignalListItemFactory> factory = invoice_information.retrieve_item();*/
/**/
/*	if (factory)*/
/*		CHECK(true);*/
/*}*/
/**/
/*TEST(statement_list_item_test, list_item_retrieve_value)*/
/*{*/
/*}*/




/*************************GUI PART COLUMN INVOICE NUMBER TEST LIST*************
 * 1) The column invoice number should have a name. (Done)
 * 3) The column invoice number should have the ability to
 *    retrieve the value. (Done)
 * 4) The column invoice number should be created. (Done)
 * 5) The column invoice number should be destroyed. (Done)
 * 6) The column invoice number should be set. (Done)
 * 7) The column must have the ability to verify it's validity. (Done)
  ******************************************************************************/
TEST_GROUP(statement_columns_test)
{
	std::string invoice_number{"INV-200"};
	std::string date{"2025-06-14"};
	std::string order_number{"MD-4"};
	std::string paid_status{"paid"};
	std::string price{"R1234.00"};
	std::string price_title{"price"};
	std::string date_title{"date"};
	std::string order_number_title{"order number"};
	std::string paid_status_title{"paid"};
	std::string invoice_number_title{"invoice number"};
	Glib::RefPtr<Gtk::Builder> builder{};
	Glib::RefPtr<Gtk::Application> app{};
	std::unique_ptr<Gtk::ColumnView> view{};
	Glib::RefPtr<Gtk::SelectionModel> selection{};
	gui::part::statement::columns::date date_column{};
	gui::part::statement::columns::price price_column{};
	gui::part::statement::columns::paid_status paid_status_column{};
	gui::part::statement::columns::order_number order_number_column{};
	gui::part::statement::columns::invoice_number invoice_number_column{};
	Glib::RefPtr<Gio::ListStore<gui::part::statement::columns::entries>> store{};
	void setup()
	{
		app = Gtk::Application::create("org.testing");
		builder = Gtk::Builder::create();
		builder->add_from_file("../gui/admin-system.ui");
		view = std::unique_ptr<Gtk::ColumnView>{
			builder->get_widget<Gtk::ColumnView>("statement-column-view")};
		store = Gio::ListStore<gui::part::statement::columns::entries>::create();
		store->append(gui::part::statement::columns::entries::create(invoice_number,
							       date,
							       order_number,
							       paid_status,
							       price));
		selection = Gtk::SingleSelection::create(store);
		view->set_model(selection);
		(void) date_column.create(date_title);
		(void) price_column.create(price_title);
		(void) paid_status_column.create(paid_status_title);
		(void) order_number_column.create(order_number_title);
		(void) invoice_number_column.create(invoice_number_title);
	}

	void teardown()
	{
		app.reset();
	}
};

TEST(statement_columns_test, check_if_the_column_is_valid_after_creation)
{
        CHECK_EQUAL(false, invoice_number_column.is_not_valid());
        CHECK_EQUAL(false, date_column.is_not_valid());
        CHECK_EQUAL(false, order_number_column.is_not_valid());
        CHECK_EQUAL(false, paid_status_column.is_not_valid());
        CHECK_EQUAL(false, price_column.is_not_valid());
}

TEST(statement_columns_test, retrieve_the_column)
{
        Glib::RefPtr<Gtk::ColumnViewColumn> invoice_number_column_temp{invoice_number_column.retrieve_item()};
        Glib::RefPtr<Gtk::ColumnViewColumn> date_column_temp{date_column.retrieve_item()};
        Glib::RefPtr<Gtk::ColumnViewColumn> order_number_column_temp{order_number_column.retrieve_item()};
        Glib::RefPtr<Gtk::ColumnViewColumn> paid_status_column_temp{paid_status_column.retrieve_item()};
        Glib::RefPtr<Gtk::ColumnViewColumn> price_column_temp{price_column.retrieve_item()};
        view->append_column(invoice_number_column.retrieve_item());
        view->append_column(date_column.retrieve_item());
        view->append_column(order_number_column.retrieve_item());
        view->append_column(paid_status_column.retrieve_item());
        view->append_column(price_column.retrieve_item());
        std::string result_invoice_number_column{invoice_number_column_temp->get_title()};
        std::string result_date_column{date_column_temp->get_title()};
        std::string result_order_number_column{order_number_column_temp->get_title()};
        std::string result_paid_status_column{paid_status_column_temp->get_title()};
        std::string result_price_column{price_column_temp->get_title()};

        CHECK_EQUAL(invoice_number_title, result_invoice_number_column);
        CHECK_EQUAL(date_title, result_date_column);
        CHECK_EQUAL(order_number_title, result_order_number_column);
        CHECK_EQUAL(paid_status_title, result_paid_status_column);
        CHECK_EQUAL(price_title, result_price_column);
}

TEST(statement_columns_test, retrieve_value)
{
        Glib::RefPtr<Gtk::ColumnViewColumn> invoice_number_column_temp{invoice_number_column.retrieve_item()};
        Glib::RefPtr<Gtk::ColumnViewColumn> date_column_temp{date_column.retrieve_item()};
        Glib::RefPtr<Gtk::ColumnViewColumn> order_number_column_temp{order_number_column.retrieve_item()};
        Glib::RefPtr<Gtk::ColumnViewColumn> paid_status_column_temp{paid_status_column.retrieve_item()};
        Glib::RefPtr<Gtk::ColumnViewColumn> price_column_temp{price_column.retrieve_item()};
        view->append_column(invoice_number_column.retrieve_item());
        view->append_column(date_column.retrieve_item());
        view->append_column(order_number_column.retrieve_item());
        view->append_column(paid_status_column.retrieve_item());
        view->append_column(price_column.retrieve_item());
        std::string expected_invoice_number{invoice_number};
        std::string expected_date{date};
        std::string expected_order_number{order_number};
        std::string expected_paid_status{paid_status};
        std::string expected_price{price};

        CHECK_EQUAL(expected_invoice_number, invoice_number_column.retrieve_value());
        CHECK_EQUAL(expected_date, date_column.retrieve_value());
        CHECK_EQUAL(expected_order_number, order_number_column.retrieve_value());
        CHECK_EQUAL(expected_paid_status, paid_status_column.retrieve_value());
        CHECK_EQUAL(expected_price, price_column.retrieve_value());
}
