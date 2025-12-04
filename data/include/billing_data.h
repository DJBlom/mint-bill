/*****************************************************************************
 * @file billing_data.h
 *
 * @brief
 *   Declares the billing data model used to represent invoice or statement
 *   level metadata such as identifiers, names, dates, and payment status.
 *
 * @details
 *   The data::billing structure encapsulates basic billing-related fields and
 *   provides a minimal interface for setting and retrieving them. A simple
 *   bitmask-based flag mechanism is used to track which fields have been
 *   populated, enabling a concise implementation of is_valid() to verify that
 *   all required attributes are present.
 *
 *   A mutex is used to protect internal mutable state, allowing instances of
 *   data::billing to be safely accessed and modified from multiple threads
 *   when integrated into a larger, concurrent application.
 *
 * @responsibilities
 *   Provide setters and getters for billing identifier, name, date, and paid
 *   status.
 *   Track field completeness through an internal flag mask.
 *   Support thread-safe updates using a mutex for synchronization.
 *
 * @notes
 *   This abstraction focuses on basic billing metadata and does not perform
 *   formatting, localization, or persistent storage. Those concerns are
 *   expected to be handled in higher layers of the system.
 *****************************************************************************/
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
