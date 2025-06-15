
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
extern "C"
{

}



/**********************************GUI PART SEARCH BAR TEST LIST***************
 * 1)
 ******************************************************************************/
TEST_GROUP(gui_part_search_bar)
{
        gui::part::search_bar search_bar{"business-name-search"};
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

TEST(gui_part_search_bar, create_search_bar)
{
	CHECK_EQUAL(true, search_bar.create(builder));
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

TEST(gui_part_search_bar, connect_search_bar)
{
	(void) search_bar.create(builder);

	CHECK_EQUAL(true, search_bar.connect());
}

TEST(gui_part_search_bar, search_value)
{
	(void) search_bar.create(builder);
	(void) search_bar.connect();

	CHECK_EQUAL("", search_bar.keyword());
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
TEST_GROUP(statement_page_column_view)
{
        Glib::RefPtr<Gtk::Builder> builder{};
        Glib::RefPtr<Gtk::Application> app{};
        gui::part::statement::column_view column_view{"statement-column-view"};
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

TEST(statement_page_column_view, column_view_create)
{
        CHECK_EQUAL(true, column_view.create(builder));
}

TEST(statement_page_column_view, column_view_is_valid)
{
        (void) column_view.create(builder);

        CHECK_EQUAL(false, column_view.is_not_valid());
}

TEST(statement_page_column_view, column_view_add_column)
{
        (void) column_view.create(builder);
        (void) column_view.is_not_valid();

        CHECK_EQUAL(true, column_view.add_column(invoice_number));
        CHECK_EQUAL(true, column_view.add_column(date));
        CHECK_EQUAL(true, column_view.add_column(order_number));
        CHECK_EQUAL(true, column_view.add_column(paid_status));
        CHECK_EQUAL(true, column_view.add_column(price));
}

TEST(statement_page_column_view, column_view_populate)
{
        feature::client_statement client_statement{};
        (void) column_view.create(builder);
        (void) column_view.is_not_valid();
        (void) column_view.add_column(invoice_number);
        (void) column_view.add_column(date);
        (void) column_view.add_column(order_number);
        (void) column_view.add_column(paid_status);
        (void) column_view.add_column(price);

        CHECK_EQUAL(true, column_view.populate(client_statement));
}

TEST(statement_page_column_view, extract_data_from_store)
{
        feature::client_statement client_statement{};
        (void) column_view.create(builder);
        (void) column_view.is_not_valid();
        (void) column_view.add_column(invoice_number);
        (void) column_view.add_column(date);
        (void) column_view.add_column(order_number);
        (void) column_view.add_column(paid_status);
        (void) column_view.add_column(price);
        (void) column_view.populate(client_statement);
	std::vector<std::any> records(column_view.extract());

        CHECK_EQUAL(100, records.size());
}





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
