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

bool gui::statement_page::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
	bool created{false};
        if (_ui_builder)
        {
		if (this->no_item_selected.create(_ui_builder) == false)
		{
			return created;
		}

		(void) this->no_item_selected.connect([this] (const int& response) {
			std::cout << "Response: " << std::to_string(response) << std::endl;
			(void) this->no_item_selected.hide();
		});

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

		if (this->statement_pdf_view.create(_ui_builder) == false)
		{
                        return created;
		}

		created = true;
        }

        return created;
}

bool gui::statement_page::search(const std::string& _keyword)
{
        bool searched{true};
        if (_keyword.empty())
        {
                syslog(LOG_CRIT, "The _keywword is empty - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		searched = false;
        }
        else
        {
		feature::client_statement client_statement{};
		std::vector<std::any> invoices{};
		std::vector<std::any> statements{};
		std::vector<std::any> pdf_statements = client_statement.load(_keyword);
		for (const std::any& data : pdf_statements)
		{
			data::pdf_statement pdf_statement{std::any_cast<data::pdf_statement>(data)};
			(void) pdf_statement;
			// statements.emplace_back(pdf_statement.get_statement());
			const std::vector<data::pdf_invoice>& pdf_invoices = pdf_statement.get_pdf_invoices();
			std::transform(pdf_invoices.begin(), pdf_invoices.end(), std::back_inserter(invoices),
				[](const data::pdf_invoice& invoice) {
				return std::any{invoice};
			});
		}

		if (this->statement_view.populate(statements) == false)
		{
			searched = false;
		}

		if (this->invoice_pdf_view.populate(invoices) == false)
		{
			searched = false;
		}

		if (this->statement_pdf_view.populate(pdf_statements) == false)
		{
			searched = false;
		}
        }

        return searched;
}

bool gui::statement_page::print()
{
	(void) this->no_item_selected.show();
	return false;
}

bool gui::statement_page::email()
{
	(void) this->no_item_selected.show();
	return false;
}

bool gui::statement_page::save()
{
	bool success{false};

	return success;
}
