/********************************************************
 * Contents: business data declaration
 * Author: Dawid J. Blom
 * Date: October 13, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _BUSINESS_DATA_H_
#define _BUSINESS_DATA_H_
#include <mutex>
#include <regex>
#include <string>
#include <cstdint>
#include <word_slicer.h>

namespace data {
struct business {
public:
	business() = default;
	business(const business& new_data);
	business(business&&);
	business& operator= (const business&);
	business& operator= (business&&);
	virtual ~business() = default;

	[[nodiscard]] virtual bool is_valid() const;
	virtual void set_name(const std::string&);
	[[nodiscard]] virtual std::string get_name() const;
	virtual void set_address(const std::string&);
	[[nodiscard]] virtual std::string get_address() const;
	virtual void set_area_code(const std::string&);
	[[nodiscard]] virtual std::string get_area_code() const;
	virtual void set_town(const std::string&);
	[[nodiscard]] virtual std::string get_town() const;
	virtual void set_cellphone(const std::string&);
	[[nodiscard]] virtual std::string get_cellphone() const;
	virtual void set_email(const std::string&);
	[[nodiscard]] virtual std::string get_email() const;

private:
	void set_flag(const int&);
	void clear_flag(const int&);
	[[nodiscard]] bool check_flags() const;
	[[nodiscard]] bool email_address_good(const std::vector<std::string>&);

private:
	using mask_type = std::uint8_t;

	std::string name{""};
	std::string address{""};
	std::string area_code{""};
	std::string town{""};
	std::string cellphone{""};
	std::string email_addresses{""};
	mask_type flags{0x0};
	std::mutex business_data{};
	mask_type mask{0x3F};
	enum FLAG {
		NAME = 0,
		ADDRESS,
		AREA_CODE,
		TOWN,
		CELLPHONE,
		EMAIL
	};

	enum BIT {
		CLEAR = 0,
		SET
	};
};
}
#endif
