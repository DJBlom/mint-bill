/********************************************************
 * Contents: date manager definition
 * Author: Dawid J. Blom
 * Date: November 28, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _DATE_MANAGER_H_
#define _DATE_MANAGER_H_
#include <string>

namespace utility {
struct period_bounds
{
    std::string period_start; // "YYYY-MM-DD"
    std::string period_end;   // "YYYY-MM-DD"
};

class date_manager {
public:
	date_manager() = default;
	date_manager(const date_manager&) = delete;
	date_manager(date_manager&&) = delete;
	date_manager& operator= (const date_manager&) = delete;
	date_manager& operator= (date_manager&&) = delete;
	virtual ~date_manager() = default;

	[[nodiscard]] virtual std::string current_date();
	[[nodiscard]] virtual period_bounds compute_period_bounds(const std::string&);


};
}
#endif
