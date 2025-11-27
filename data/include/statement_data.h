/********************************************************
 * Contents: Statement data definition
 * Author: Dawid J. Blom
 * Date: November 27, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _STATEMENT_DATA_H_
#define _STATEMENT_DATA_H_
#include <mutex>
#include <regex>
#include <vector>
#include <string>
#include <cstdint>

namespace data {
struct statement {
public:
	statement();
	statement(const statement&);
	statement(statement&&);
	statement& operator= (const statement&);
	statement& operator= (statement&&);
	virtual ~statement();

	[[nodiscard]] virtual bool is_valid() const;
	virtual void set_business_name(const std::string&);
	[[nodiscard]] virtual std::string get_business_name() const;
	virtual void set_period_start(const std::string&);
	[[nodiscard]] virtual std::string get_period_start() const;
	virtual void set_period_end(const std::string&);
	[[nodiscard]] virtual std::string get_period_end() const;
	virtual void set_schedule(const std::string&);
	[[nodiscard]] virtual std::string get_schedule() const;
	virtual void set_statement_date(const std::string&);
	[[nodiscard]] virtual std::string get_statement_date() const;

private:
	void set_flag(const int&);
	void clear_flag(const int&);
	[[nodiscard]] bool check_flags() const;

private:
	using mask_type = std::uint8_t;

	std::string business_name{""};
	std::string period_start{""};
	std::string period_end{""};
	std::string schedule{""};
	std::string statement_date{""};
	mask_type flags{0x0};
	std::mutex statement_data{};
	mask_type mask{0x1F};
	enum FLAG {
		NAME = 0,
		PERIOD_START,
		PERIOD_END,
		SCHEDULE,
		DATE
	};

	enum BIT {
		CLEAR = 0,
		SET
	};
};
}
#endif
