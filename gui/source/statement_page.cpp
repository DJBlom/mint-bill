/*******************************************************
 * Contents: Statement Page class declaration
 * Author: Dawid J. Blom
 * Date: February 19, 2025
 *
 * NOTE:
 *******************************************************/
#include <statement_page.h>
#include <client_invoice.h>
#include <client_statement.h>
#include <pdf_statement_data.h>

#include <iostream>

gui::statement_page::~statement_page() {}

bool gui::statement_page::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder, const interface::observer& _search_bar)
{
	bool created{false};
        if (_ui_builder)
        {
                // save
                if (this->save_button.create(_ui_builder) == false)
                {
                        return created;
                }

                if (this->save_dialog.create(_ui_builder) == false)
                {
                        return created;
                }

                if (this->save_dialog.connect() == false)
                {
                        return created;
                }

                if (this->save_button.connect(this->save_dialog) == false)
                {
                        return created;
                }

                // print
                if (this->print_button.create(_ui_builder) == false)
                {
                        return created;
                }

                if (this->print_dialog.create(_ui_builder) == false)
                {
                        return created;
                }

                if (this->print_dialog.connect() == false)
                {
                        return created;
                }

                if (this->print_button.connect(this->print_dialog) == false)
                {
                        return created;
                }

                this->print_button.disable();

                // email
                if (this->email_button.create(_ui_builder) == false)
                {
                        return created;
                }

                if (this->email_dialog.create(_ui_builder) == false)
                {
                        return created;
                }

                if (this->email_dialog.connect() == false)
                {
                        return created;
                }

                if (this->email_button.connect(this->email_dialog) == false)
                {
                        return created;
                }

                this->email_button.disable();

		if (this->statement_view.create(_ui_builder) == false)
		{
                        return created;
		}

		(void) this->date.create("Date");
		(void) this->price.create("Price");
		(void) this->paid_status.create("Paid Status");
		(void) this->order_number.create("Order Number");
		(void) this->invoice_number.create("Invoice Number");

		if (this->statement_view.add_column(invoice_number) == false)
		{
                        return created;
		}
		if (this->statement_view.add_column(date) == false)
		{
                        return created;
		}
		if (this->statement_view.add_column(order_number) == false)
		{
                        return created;
		}
		if (this->statement_view.add_column(paid_status) == false)
		{
                        return created;
		}
		if (this->statement_view.add_column(price) == false)
		{
                        return created;
		}

		if (this->invoice_pdf_view.create(_ui_builder) == false)
		{
                        return created;
		}


		feature::client_statement client_statement{};
		created = _search_bar.subscribe("statement-page", [this, client_statement](const std::string& _keyword) {
			bool success{true};
			std::vector<std::any> invoices{};
			std::vector<std::any> statements{};
			for (const std::any& data : client_statement.load(_keyword))
			{
				data::pdf_statement pdf_statement{std::any_cast<data::pdf_statement>(data)};
				statements.emplace_back(pdf_statement.get_statement());
				const std::vector<data::pdf_invoice>& pdf_invoices = pdf_statement.get_pdf_invoices();
				std::transform(pdf_invoices.begin(), pdf_invoices.end(), std::back_inserter(invoices),
					[](const data::pdf_invoice& invoice) {
					return std::any{invoice};
				});
			}

			if (this->statement_view.populate(statements) == false)
			{
				success = false;
			}

			if (this->invoice_pdf_view.populate(invoices) == false)
			{
				success = false;
			}

			return success;
		});
        }

        return created;
}
