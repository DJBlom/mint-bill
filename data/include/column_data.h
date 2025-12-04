/*****************************************************************************
 * @file column_data.h
 *
 * @brief
 *   Declares the column data model representing a single logical row entry
 *   with quantity, description, amount, row index, and a description flag.
 *
 * @details
 *   The data::column structure encapsulates the fields required to describe an
 *   individual line item in a tabular dataset, such as an invoice or report.
 *   It stores numeric and textual attributes and exposes a small interface for
 *   setting and retrieving each one. A bitmask-based flag mechanism is used to
 *   track which fields are initialized, allowing is_valid() to provide a
 *   concise completeness check.
 *
 *   The model also includes a logical indicator field (is_description) that
 *   distinguishes between descriptive rows and value-bearing rows. A mutex is
 *   used to protect internal state during updates, supporting safe use in
 *   multi-threaded scenarios.
 *
 * @responsibilities
 *   Represent an individual tabular entry with quantity, description, amount,
 *   row number, and a logical flag.
 *   Provide setters and getters for each stored attribute.
 *   Track initialization state via an internal flag mask.
 *   Support thread-safe modifications using a per-instance mutex.
 *
 * @notes
 *   Logical constants LOGICAL_TRUE and LOGICAL_FALSE are defined at namespace
 *   scope to indicate the expected values for the is_description field. Higher
 *   layers can interpret this flag when rendering or processing tabular data.
 *****************************************************************************/
#ifndef _COLUMN_DATA_H_
#define _COLUMN_DATA_H_
#include <mutex>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

namespace data {
constexpr std::uint32_t LOGICAL_TRUE{1};
constexpr std::uint32_t LOGICAL_FALSE{0};

struct column {
public:
	column();
	column(const column& new_data);
	column(column&&);
	column& operator= (const column&);
	column& operator= (column&&);
	virtual ~column();

	[[nodiscard]] virtual bool is_valid() const;
	virtual void set_quantity(const unsigned int&);
	[[nodiscard]] virtual unsigned int get_quantity() const;
	virtual void set_description(const std::string&);
	[[nodiscard]] virtual std::string get_description() const;
	virtual void set_amount(const double&);
	[[nodiscard]] virtual double get_amount() const;
	virtual void set_row_number(const long long&);
	[[nodiscard]] virtual long long get_row_number() const;
	virtual void set_is_description(const long long&);
	[[nodiscard]] virtual long long get_is_description() const;

private:
	void set_flag(const int&);
	void clear_flag(const int&);
	[[nodiscard]] bool check_flags() const;

private:
	using mask_type = std::uint8_t;

	unsigned int quantity{0};
	std::string description{""};
	double amount{0.0};
	long long row_number{0};
	long long is_description{0};
	mask_type flags{0x0};
	std::mutex column_data{};
	mask_type mask{0x1F};

	enum FLAG {
		QUANTITY = 0,
		DESCRIPTION,
		AMOUNT,
		ROW_NUMBER,
		IS_DESCRIPTION
	};

	enum BIT {
		CLEAR = 0,
		SET
	};
};
}
#endif
