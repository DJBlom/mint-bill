/********************************************************
 * Contents: Statement Page class declaration
 * Author: Dawid J. Blom
 * Date: February 19, 2025
 *
 * NOTE:
 *******************************************************/
#include <statement_page.h>
#include <client_statement.h>

gui::statement_page::~statement_page() {}

bool gui::statement_page::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        if (_ui_builder)
        {
                // save
                if (this->save_button.create(_ui_builder) == false)
                {
                        return false;
                }

                if (this->save_dialog.create(_ui_builder) == false)
                {
                        return false;
                }

                if (this->save_dialog.connect() == false)
                {
                        return false;
                }

                if (this->save_button.connect(this->save_dialog) == false)
                {
                        return false;
                }

                // print
                if (this->print_button.create(_ui_builder) == false)
                {
                        return false;
                }

                if (this->print_dialog.create(_ui_builder) == false)
                {
                        return false;
                }

                if (this->print_dialog.connect() == false)
                {
                        return false;
                }

                if (this->print_button.connect(this->print_dialog) == false)
                {
                        return false;
                }

                this->print_button.disable();

                // email
                if (this->email_button.create(_ui_builder) == false)
                {
                        return false;
                }

                if (this->email_dialog.create(_ui_builder) == false)
                {
                        return false;
                }

                if (this->email_dialog.connect() == false)
                {
                        return false;
                }

                if (this->email_button.connect(this->email_dialog) == false)
                {
                        return false;
                }

                this->email_button.disable();

		if (this->statement_view.create(_ui_builder) == false)
		{
			return false;
		}

		part::statement::columns::invoice_number invoice_number{"invoice number"};
		(void) this->statement_view.add_column(invoice_number);
		feature::client_statement client_statement{};
		if (this->statement_view.is_not_valid())
			return false;
		/*else*/
		/*	(void) this->statement_view.populate(client_statement);*/
        }

        return true;
}
