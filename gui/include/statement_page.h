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
#include <thread>
#include <future>
#include <syslog.h>
#include <gui_parts.h>
#include <email_data.h>
#include <statement_pdf.h>
#include <statement_model.h>


namespace gui {
class statement_page : public interface::operations_page {
public:
        statement_page();
        statement_page(const statement_page&) = delete;
        statement_page(statement_page&&) = delete;
        statement_page& operator= (const statement_page&) = delete;
        statement_page& operator= (statement_page&&) = delete;
        ~statement_page() override;

	[[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&,
					  const std::shared_ptr<Gtk::Window>& _main_window) override;
        [[nodiscard]] virtual bool set_database_password(const std::string&) override;
	[[nodiscard]] virtual bool search(const std::string&) override;
	[[nodiscard]] virtual bool print() override;
	[[nodiscard]] virtual bool email() override;
	[[nodiscard]] virtual bool save() override;

protected:
	void email_sent();
	bool email_setup(const Glib::RefPtr<Gtk::Builder>&);
	bool print_setup(const Glib::RefPtr<Gtk::Builder>&,
			 const std::shared_ptr<Gtk::Window>&);
	bool save_setup(const Glib::RefPtr<Gtk::Builder>&);
	bool no_item_selected_setup(const Glib::RefPtr<Gtk::Builder>&);
	bool no_internet_setup(const Glib::RefPtr<Gtk::Builder>&);
	bool statement_column_view_setup(const Glib::RefPtr<Gtk::Builder>&);
	bool invoice_pdf_view_setup(const Glib::RefPtr<Gtk::Builder>&);
	bool statement_pdf_view_setup(const Glib::RefPtr<Gtk::Builder>&);

private:
	std::future<bool> email_future;
	Glib::Dispatcher email_dispatcher{};
	bool on_double_click();
	bool on_single_click();


private:
	std::string database_password{""};
	std::vector<std::any> documents{};
	std::vector<std::any> invoice_data{};
	data::pdf_statement selected_pdf_statement{};
	std::unique_ptr<Gtk::Label> total_label{};
        part::dialog no_item_selected{"statement-no-item-selected-alert"};
        part::dialog email_alert{"statement-email-button-alert"};
        part::dialog print_alert{"statement-print-button-alert"};
        part::dialog no_internet_alert{"statement-email-no-internet-alert"};
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
