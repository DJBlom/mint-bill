/********************************************************
 * Contents: admin data definition
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
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
	virtual ~admin() = default;

	[[nodiscard]] virtual bool is_valid() const override;
	virtual void set_name(const std::string&) override;
	[[nodiscard]] virtual std::string get_name() const override;
	virtual void set_address(const std::string&) override;
	[[nodiscard]] virtual std::string get_address() const override;
	virtual void set_area_code(const std::string&) override;
	[[nodiscard]] virtual std::string get_area_code() const override;
	virtual void set_town(const std::string&) override;
	[[nodiscard]] virtual std::string get_town() const override;
	virtual void set_cellphone(const std::string&) override;
	[[nodiscard]] virtual std::string get_cellphone() const override;
	virtual void set_email(const std::string&) override;
	[[nodiscard]] virtual std::string get_email() const override;
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
