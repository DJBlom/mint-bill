/*****************************************************************************
 * @file statement_data.h
 *
 * @brief
 *   Declares the statement data model that extends billing information with
 *   statement-specific period boundaries.
 *
 * @details
 *   The data::statement structure derives from data::billing to reuse common
 *   billing metadata such as identifier, name, date, and paid status. It
 *   augments this with a start and end period, representing the date range
 *   covered by a financial statement.
 *
 *   A bitmask-based flag mechanism is used to track initialization of the
 *   period fields, enabling is_valid() to combine statement-level completeness
 *   with the underlying billing validation logic. A mutex guards modifications
 *   to internal state, allowing safe use in multi-threaded environments.
 *
 * @responsibilities
 *   Extend billing data with statement period boundaries.
 *   Provide setters and getters for period start and end values.
 *   Track initialization state using an internal flag mask.
 *   Support thread-safe updates via a per-instance mutex.
 *
 * @notes
 *   This abstraction focuses on logical statement metadata. Formatting,
 *   presentation, and persistence are expected to be handled by higher-level
 *   components that consume the statement data.
 *****************************************************************************/
#ifndef _STATEMENT_DATA_H_
#define _STATEMENT_DATA_H_
#include <mutex>
#include <regex>
#include <vector>
#include <string>
#include <cstdint>
#include <billing_data.h>

namespace data {
struct statement : public billing {
public:
	statement();
	statement(const statement&);
	statement(statement&&);
	statement& operator= (const statement&);
	statement& operator= (statement&&);
	virtual ~statement() override;

	[[nodiscard]] virtual bool is_valid() const override;
	using data::billing::set_id;
	using data::billing::get_id;
	using data::billing::set_name;
	using data::billing::get_name;
	using data::billing::set_date;
	using data::billing::get_date;
	using data::billing::set_paid_status;
	using data::billing::get_paid_status;
	virtual void set_period_start(const std::string&);
	[[nodiscard]] virtual std::string get_period_start() const;
	virtual void set_period_end(const std::string&);
	[[nodiscard]] virtual std::string get_period_end() const;

private:
	void set_flag(const int&);
	void clear_flag(const int&);
	[[nodiscard]] bool check_flags() const;

private:
	using mask_type = std::uint8_t;

	std::string period_start{""};
	std::string period_end{""};
	mask_type flags{0x0};
	std::mutex statement_data{};
	mask_type mask{0x3};
	enum FLAG {
		PERIOD_START = 0,
		PERIOD_END,
	};

	enum BIT {
		CLEAR = 0,
		SET
	};
};
}
#endif
