/********************************************************
 * Contents: Invoice data definition
 * Author: Dawid J. Blom
 * Date: December 4, 2024
 *
 * NOTE:
 *******************************************************/
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
