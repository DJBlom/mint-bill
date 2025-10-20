/********************************************************
 * Contents: Client data definition
 * Author: Dawid J. Blom
 * Date: November 26, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _CLIENT_DATA_H_
#define _CLIENT_DATA_H_
#include <mutex>
#include <regex>
#include <string>
#include <vector>
#include <cstdint>
#include <business_data.h>

namespace data {
struct client : public data::business {
public:
	client() = default;
	client(const client& new_data);
	client(client&&);
	client& operator= (const client&);
	client& operator= (client&&);
	virtual ~client() override;

	[[nodiscard]] virtual bool is_valid() const override;
	using data::business::set_name;
	using data::business::get_name;
	using data::business::set_address;
	using data::business::get_address;
	using data::business::set_area_code;
	using data::business::get_area_code;
	using data::business::set_town;
	using data::business::get_town;
	using data::business::set_cellphone;
	using data::business::get_cellphone;
	using data::business::set_email;
	using data::business::get_email;
	virtual void set_vat_number(const std::string&);
	[[nodiscard]] virtual std::string get_vat_number() const;
	virtual void set_statement_schedule(const std::string&);
	[[nodiscard]] virtual std::string get_statement_schedule() const;

private:
	void set_flag(const int&);
	void clear_flag(const int&);
	[[nodiscard]] bool check_flags() const;

private:
	using mask_type = std::uint8_t;

	std::string vat_number{""};
	std::string statement_schedule{""};
	mask_type flags{0x0};
	std::mutex client_data{};
	mask_type mask{0x3};
	enum FLAG {
		VAT_NUMBER = 0,
		STATMENT_SCHEDULE
	};

	enum BIT {
		CLEAR = 0,
		SET
	};
};
}
#endif
