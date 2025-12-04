/*****************************************************************************
 * @file admin_data.h
 *
 * @brief
 *   Defines the administrative data model extending the core business
 *   information structure. This model introduces additional attributes
 *   required for administrative configuration and secure profile management.
 *
 * @details
 *   The admin class provides controlled access to sensitive data such as
 *   banking details, account identifiers, passwords, and client-facing
 *   messages. It inherits standard business information and augments it with
 *   administrative attributes used by higher-level application logic.
 *
 *   A bitmask-based flag system tracks field completeness, enabling validation
 *   of required attributes. A mutex protects internal state to ensure safe
 *   concurrent modification.
 *
 * @responsibilities
 *   Encapsulate and manage administrative attributes.
 *   Extend and reuse base business information.
 *   Provide overrideable accessors for integration flexibility.
 *   Track field validity using internal flags.
 *   Support thread-safe updates to internal data.
 *
 * @notes
 *   Suitable for components handling administrative profiles, configuration
 *   workflows, or authentication-related data management.
 *****************************************************************************/
#ifndef _ADMIN_DATA_H_
#define _ADMIN_DATA_H_
#include <mutex>
#include <regex>
#include <string>
#include <cstdint>
#include <business_data.h>

namespace data {
struct admin : public data::business {
public:
	admin() = default;
	admin(const admin& new_data);
	admin(admin&&);
	admin& operator= (const admin&);
	admin& operator= (admin&&);
	virtual ~admin() override;

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
	virtual void set_bank(const std::string&);
	[[nodiscard]] virtual std::string get_bank() const;
	virtual void set_branch_code(const std::string&);
	[[nodiscard]] virtual std::string get_branch_code() const;
	virtual void set_account_number(const std::string&);
	[[nodiscard]] virtual std::string get_account_number() const;
	virtual void set_client_message(const std::string&);
	[[nodiscard]] virtual std::string get_client_message() const;
	virtual void set_password(const std::string&);
	[[nodiscard]] virtual std::string get_password() const;

private:
	void set_flag(const int&);
	void clear_flag(const int&);
	[[nodiscard]] bool check_flags() const;

private:
	using mask_type = std::uint8_t;

	std::string bank{""};
	std::string branch_code{""};
	std::string account_number{""};
	std::string client_message{""};
	std::string password{""};
	mask_type flags{0x0};
	std::mutex business_data{};
	mask_type mask{0x1F};
	enum FLAG {
		BANK = 0,
		BRANCH_CODE,
		ACCOUNT_NUMBER,
		CLIENT_MESSAGE,
		PASSWORD
	};

	enum BIT {
		CLEAR = 0,
		SET
	};
};
}
#endif
