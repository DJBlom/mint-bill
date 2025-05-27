
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
#include <statement_page.h>
extern "C"
{

}




/**********************************GUI PART DIALOG TEST LIST*******************
 * 1)
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

TEST(gui_part_dialog, create_dialog)
{
        CHECK_EQUAL(true, gui_dialog.create(builder));
}

TEST(gui_part_dialog, check_if_the_dialog_is_valid)
{
        CHECK_EQUAL(true, gui_dialog.is_not_valid());
}

TEST(gui_part_dialog, connect_dialog_event_handler_successfully)
{
        CHECK_EQUAL(true, gui_dialog.create(builder));
        CHECK_EQUAL(true, gui_dialog.connect());
}

TEST(gui_part_dialog, connect_dialog_event_handler_unsuccessfully)
{
        CHECK_EQUAL(false, gui_dialog.connect());
}

TEST(gui_part_dialog, show_the_dialog)
{
        CHECK_EQUAL(true, gui_dialog.create(builder));
        CHECK_EQUAL(true, gui_dialog.connect());
        gui_dialog.show();
}







/**********************************GUI PART BUTTON TEST LIST*******************
 * 1)
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

TEST(gui_part_button, create_button)
{
        CHECK_EQUAL(true, gui_button.create(builder));
}

TEST(gui_part_button, connect_button_event_handler)
{
        CHECK_EQUAL(true, dialog.create(builder));
        CHECK_EQUAL(true, gui_button.create(builder));
        CHECK_EQUAL(true, gui_button.connect(dialog));
}

TEST(gui_part_button, disable_the_button)
{
        CHECK_EQUAL(true, dialog.create(builder));
        CHECK_EQUAL(true, gui_button.create(builder));
        CHECK_EQUAL(true, gui_button.connect(dialog));
        gui_button.disable();
}

TEST(gui_part_button, enable_the_button)
{
        CHECK_EQUAL(true, dialog.create(builder));
        CHECK_EQUAL(true, gui_button.create(builder));
        CHECK_EQUAL(true, gui_button.connect(dialog));
        gui_button.enable();
}
