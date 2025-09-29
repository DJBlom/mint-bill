/********************************************************
 * Contents: admin Information Page class definition
 * Author: Dawid J. Blom
 * Date: November 14, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _ADMIN_PAGE_H_
#define _ADMIN_PAGE_H_
#include <gui.h>
#include <admin_model.h>

namespace gui {
//GCOVR_EXCL_START
class admin_page : public interface::register_page {
public:
	admin_page() = default;
	admin_page(const admin_page&) = delete;
	admin_page(admin_page&&) = delete;
	admin_page& operator= (const admin_page&) = delete;
	admin_page& operator= (admin_page&&) = delete;
	~admin_page() override;

	[[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;
        [[nodiscard]] virtual bool set_database_password(const std::string&) override;
	[[nodiscard]] virtual bool search(const std::string&) override;
	[[nodiscard]] virtual bool save() override;

private:
	void create_entries(const Glib::RefPtr<Gtk::Builder>&);
	void connect_wrong_info_alert();
	void connect_save_alert();
	void clear_entries();
	void update_business_info_with_db_data(const std::string&);
	data::admin extract_page_entries();

private:
	std::string database_password{""};

private: // Label
	std::unique_ptr<Gtk::Label> organization_label{};

private: // Entries
	std::unique_ptr<Gtk::Entry> name{};
	std::unique_ptr<Gtk::Entry> street_address{};
	std::unique_ptr<Gtk::Entry> area_code{};
	std::unique_ptr<Gtk::Entry> town_name{};
	std::unique_ptr<Gtk::Entry> cellphone{};
	std::unique_ptr<Gtk::Entry> email{};
	std::unique_ptr<Gtk::Entry> bank_name{};
	std::unique_ptr<Gtk::Entry> branch_code{};
	std::unique_ptr<Gtk::Entry> account_number{};
	std::unique_ptr<Gtk::Entry> client_message{};
	std::unique_ptr<Gtk::PasswordEntry> password{};

private: // Dialogs
	std::unique_ptr<Gtk::MessageDialog> wrong_info_alert_dialog{};
	std::unique_ptr<Gtk::MessageDialog> save_alert_dialog{};
};

//GCOVR_EXCL_STOP
}
#endif
