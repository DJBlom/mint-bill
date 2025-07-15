/********************************************************
 * Contents: Statement Page class definition
 * Author: Dawid J. Blom
 * Date: February 19, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _STATEMENT_PAGE_H_
#define _STATEMENT_PAGE_H_
#include <gui.h>
#include <syslog.h>
#include <gui_parts.h>
#include <statement_pdf.h>
#include <client_statement.h>


namespace gui {
class statement_page : public interface::operations_page {
public:
        statement_page();
        statement_page(const statement_page&) = delete;
        statement_page(statement_page&&) = delete;
        statement_page& operator= (const statement_page&) = delete;
        statement_page& operator= (statement_page&&) = delete;
        ~statement_page() override;

	[[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;
	[[nodiscard]] virtual bool search(const std::string&) override;
	[[nodiscard]] virtual bool print() override;
	[[nodiscard]] virtual bool email() override;
	[[nodiscard]] virtual bool save() override;

private:
	feature::statement_pdf statement_pdf{};
	feature::client_statement client_statement{};
	std::vector<std::shared_ptr<poppler::document>> documents{};
        part::dialog no_item_selected{"statement-no-item-selected-alert"};
        part::dialog email_alert{"statement-email-button-alert"};
        part::dialog print_alert{"statement-print-button-alert"};
        part::dialog no_printer_alert{"statement-print-no-printer-alert"};
        part::dialog save_alert{"statement-save-button-alert"};
	part::statement::columns::date date{};
	part::statement::columns::price price{};
	part::statement::columns::paid_status paid_status{};
	part::statement::columns::order_number order_number{};
	part::statement::columns::invoice_number invoice_number{};
	part::statement::column_view statement_view{"statement-column-view",
		"statement-column-view-vadjustment"};
	part::statement::invoice_pdf_view invoice_pdf_view{"statement-invoice-list-view",
		"statement-invoice-list-view-vadjustment"};
	part::statement::statement_pdf_view statement_pdf_view{"statement-list-view",
		"statement-list-view-vadjustment"};
};
}


#endif
