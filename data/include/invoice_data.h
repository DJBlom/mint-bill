/*****************************************************************************
 * @file invoice_data.h
 *
 * @brief
 *   Declares the invoice data model that extends billing information with
 *   invoice-specific identifiers, totals, and line-item column data.
 *
 * @details
 *   The data::invoice structure derives from data::billing to reuse common
 *   billing metadata such as identifier, name, date, and paid status. It
 *   augments this with invoice-specific attributes including job card number,
 *   order number, description and material totals, and a grand total. It also
 *   maintains two collections of data::column instances representing
 *   description and material line items.
 *
 *   Internal completeness is tracked using a bitmask-based flag mechanism,
 *   enabling is_valid() to combine invoice-level flags with the underlying
 *   billing validation logic. A mutex is used to guard updates to mutable
 *   state, allowing safe use in multi-threaded code.
 *
 * @responsibilities
 *   Extend billing data with invoice-specific identifiers and totals.
 *   Manage collections of description and material line-item columns.
 *   Provide setters and getters for all invoice-related attributes.
 *   Track initialization state using an internal flag mask.
 *   Support thread-safe updates via a per-instance mutex.
 *
 * @notes
 *   This abstraction represents the logical structure of an invoice. Rendering,
 *   numbering schemes, currency formatting, and persistence are expected to be
 *   implemented by higher-level components.
 *****************************************************************************/
#ifndef _INVOICE_DATA_H_
#define _INVOICE_DATA_H_
#include <mutex>
#include <regex>
#include <vector>
#include <string>
#include <cstdint>
#include <column_data.h>
#include <billing_data.h>

namespace data {
struct invoice : public billing {
public:
	invoice();
	invoice(const invoice&);
	invoice(invoice&&);
	invoice& operator= (const invoice&);
	invoice& operator= (invoice&&);
	virtual ~invoice() override;

	[[nodiscard]] virtual bool is_valid() const override;
	using data::billing::set_id;
	using data::billing::get_id;
	using data::billing::set_name;
	using data::billing::get_name;
	using data::billing::set_date;
	using data::billing::get_date;
	using data::billing::set_paid_status;
	using data::billing::get_paid_status;
	virtual void set_job_card_number(const std::string&);
	[[nodiscard]] std::string get_job_card_number() const;
	virtual void set_order_number(const std::string&);
	[[nodiscard]] std::string get_order_number() const;
	virtual void set_description_total(const std::string&);
	[[nodiscard]] std::string get_description_total() const;
	virtual void set_material_total(const std::string&);
	[[nodiscard]] std::string get_material_total() const;
	virtual void set_grand_total(const std::string&);
	[[nodiscard]] std::string get_grand_total() const;
	virtual void set_description_column(const std::vector<data::column>&);
	[[nodiscard]] std::vector<data::column> get_description_column() const;
	virtual void set_material_column(const std::vector<data::column>&);
	[[nodiscard]] std::vector<data::column> get_material_column() const;

private:
	void set_flag(const int&);
	void clear_flag(const int&);
	[[nodiscard]] bool check_flags() const;

private:
	using mask_type = std::uint8_t;

	std::string job_card_number{""};
	std::string order_number{""};
	std::string description_total{""};
	std::string material_total{""};
	std::string grand_total{""};
	std::vector<data::column> description_column{};
	std::vector<data::column> material_column{};
	mask_type flags{0x0};
	std::mutex invoice_data{};
	mask_type mask{0x7F};
	enum FLAG {
		JOB_CARD = 0,
		ORDER_NUMBER,
		DESCRIPTION_TOTAL,
		MATERIAL_TOTAL,
		GRAND_TOTAL,
		DESCRIPTION_COL,
		MATERIAL_COL
	};

	enum BIT {
		CLEAR = 0,
		SET
	};
};
}
#endif
