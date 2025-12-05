/*****************************************************************************
 * @file client_data.h
 *
 * @brief
 *   Declares the client data model, extending the core business information
 *   with client-specific attributes such as VAT number and statement schedule.
 *
 * @details
 *   The data::client structure derives from data::business to reuse common
 *   business identity and contact fields, and augments them with additional
 *   client-specific properties. These include a VAT registration number and a
 *   configurable statement schedule used to determine how often and on which
 *   day statements are generated.
 *
 *   Internal completeness is tracked using a bitmask-based flag mechanism,
 *   enabling a concise implementation of is_valid() that combines client-level
 *   flags with the base business validation logic. A mutex is used to protect
 *   internal mutable state, providing basic thread safety when the object is
 *   accessed concurrently.
 *
 * @responsibilities
 *   Extend data::business with client-specific fields.
 *   Provide setters and getters for VAT number and statement schedule.
 *   Enforce basic validation rules for client attributes.
 *   Track initialization state using an internal flag mask.
 *   Support thread-safe updates using a client-specific mutex.
 *
 * @notes
 *   The statement schedule format is validated externally via the
 *   implementation, allowing a compact encoding (for example, period and
 *   weekday) while keeping the public interface simple.
 *****************************************************************************/
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
