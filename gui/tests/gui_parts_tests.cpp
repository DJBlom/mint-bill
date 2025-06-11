
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
        CHECK_EQUAL(true, gui_dialog.is_not_valid());
}

TEST(gui_part_dialog, dialog_connect_successfully)
{
        CHECK_EQUAL(true, gui_dialog.create(builder));
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
 * 3) Extract data from the column view.
 * 4) Populate the column view.
 * 5) Clear the column view.
 * 6) Ensure the column view is valid. (Done)
 * 7) Add a column to the column view. (Done)
 * 8) Add a single data line to the column view.
 ******************************************************************************/
/*namespace feature {*/
/*class statement {*/
/*public:*/
/*        statement() = default;*/
/*        statement(const statement&) = delete;*/
/*        statement(statement&&) = delete;*/
/*        statement& operator= (const statement&) = delete;*/
/*        statement& operator= (statement&&) = delete;*/
/*        virtual ~statement() = default;*/
/**/
/*        [[nodiscard]] virtual data::statement_column load() const;*/
/*};*/
/*}*/
TEST_GROUP(statement_page_column_view)
{
        Glib::RefPtr<Gtk::Builder> builder{};
        Glib::RefPtr<Gtk::Application> app{};
        gui::part::statement::column_view column_view{"statement-column-view"};
        gui::part::statement::columns::invoice_number invoice_number{"invoice number"};
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
}

TEST(statement_page_column_view, column_view_populate)
{
        feature::client_statement client_statement{};
        (void) column_view.create(builder);

        CHECK_EQUAL(true, column_view.populate(client_statement));
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
	std::string date_title{"date"};
	std::string order_number_title{"date"};
        std::string invoice_number_title{"invoice number"};
        Glib::RefPtr<Gtk::Builder> builder{};
        Glib::RefPtr<Gtk::Application> app{};
        std::unique_ptr<Gtk::ColumnView> view{};
        Glib::RefPtr<Gtk::SelectionModel> selection{};
        gui::part::statement::columns::date date_column{date_title};
        gui::part::statement::columns::order_number order_number_column{order_number_title};
        gui::part::statement::columns::invoice_number invoice_number_column{invoice_number_title};
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
}

TEST(statement_columns_test, retrieve_the_column)
{
        Glib::RefPtr<Gtk::ColumnViewColumn> invoice_number_column_temp{invoice_number_column.retrieve_item()};
        Glib::RefPtr<Gtk::ColumnViewColumn> date_column_temp{date_column.retrieve_item()};
        view->append_column(invoice_number_column.retrieve_item());
        view->append_column(date_column.retrieve_item());
        std::string result_invoice_number_column{invoice_number_column_temp->get_title()};
        std::string result_date_column{date_column_temp->get_title()};

        CHECK_EQUAL(invoice_number_title, result_invoice_number_column);
        CHECK_EQUAL(date_title, result_date_column);
}

TEST(statement_columns_test, retrieve_value)
{
        Glib::RefPtr<Gtk::ColumnViewColumn> invoice_number_column_temp{invoice_number_column.retrieve_item()};
        Glib::RefPtr<Gtk::ColumnViewColumn> date_column_temp{date_column.retrieve_item()};
        view->append_column(invoice_number_column.retrieve_item());
        view->append_column(date_column.retrieve_item());
        std::string expected_invoice_number_column{invoice_number};
        std::string expected_date_column{date};

        CHECK_EQUAL(expected_invoice_number_column, invoice_number_column.retrieve_value());
        CHECK_EQUAL(expected_date_column, date_column.retrieve_value());
}
