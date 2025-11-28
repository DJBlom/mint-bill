/********************************************************
 * Contents: Billing data definition
 * Author: Dawid J. Blom
 * Date: November 27, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _BILLING_DATA_H_
#define _BILLING_DATA_H_
#include <mutex>
#include <vector>
#include <string>
#include <cstdint>

namespace data {
struct billing {
public:
	billing();
	billing(const billing&);
	billing(billing&&);
	billing& operator= (const billing&);
	billing& operator= (billing&&);
	virtual ~billing();

	[[nodiscard]] virtual bool is_valid() const;
	virtual void set_id(const std::string&);
	[[nodiscard]] virtual std::string get_id() const;
	virtual void set_name(const std::string&);
	[[nodiscard]] virtual std::string get_name() const;
	virtual void set_date(const std::string&);
	[[nodiscard]] virtual std::string get_date() const;
	virtual void set_paid_status(const std::string&);
	[[nodiscard]] virtual std::string get_paid_status() const;

private:
	void set_flag(const int&);
	void clear_flag(const int&);
	[[nodiscard]] bool check_flags() const;

private:
	using mask_type = std::uint8_t;

	std::string id{""};
	std::string name{""};
	std::string date{""};
	std::string paid_status{""};
	mask_type flags{0x0};
	std::mutex billing_data{};
	mask_type mask{0xF};
	enum FLAG {
		ID = 0,
		NAME,
		DATE,
		PAID_STATUS
	};

	enum BIT {
		CLEAR = 0,
		SET
	};
};
}
#endif
