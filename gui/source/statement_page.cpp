/********************************************************
 * Contents: Statement Page class declaration
 * Author: Dawid J. Blom
 * Date: February 19, 2025
 *
 * NOTE:
 *******************************************************/
#include <iostream>
#include <statement_page.h>
#include <client_statement.h>

gui::statement_page::~statement_page() {}

//bool gui::statement_page::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
bool gui::statement_page::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder, const interface::search& _search_bar)
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

		(void) this->date.create("Date");
		(void) this->price.create("Price");
		(void) this->paid_status.create("Paid Status");
		(void) this->order_number.create("Order Number");
		(void) this->invoice_number.create("Invoice Number");

		if (this->statement_view.add_column(invoice_number) == false)
		{
			return false;
		}
		if (this->statement_view.add_column(date) == false)
		{
			return false;
		}
		if (this->statement_view.add_column(order_number) == false)
		{
			return false;
		}
		if (this->statement_view.add_column(paid_status) == false)
		{
			return false;
		}
		if (this->statement_view.add_column(price) == false)
		{
			return false;
		}

		feature::client_statement client_statement{};
		_search_bar.subscribe("statements-page", [this, client_statement](const std::string& _keyword) {
			bool success{true};
			if (this->statement_view.populate(client_statement.load(_keyword)) == false)
			{
				success = false;
			}

			return success;
		});
        }

        return true;
}
