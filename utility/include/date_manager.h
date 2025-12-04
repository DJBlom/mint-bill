/*******************************************************************************
 * @file    date_manager.h
 * @brief   Date utilities for computing current date and statement period bounds.
 *
 * @details This component provides lightweight calendar and date-range utilities
 *          used throughout the billing and statement-generation workflow.
 *
 *          Features:
 *            • current_date()
 *                Returns the current local date formatted as MM-DD-YYYY.
 *
 *            • compute_period_bounds(schedule)
 *                Parses a schedule string of the form "N,D" and computes the
 *                corresponding date interval. The semantics of the schedule are:
 *
 *                  N = 1   → Weekly period based on Monday-aligned weeks.
 *                  N = 2–4 → Month-based billing periods using a defined N-th
 *                            full week and D-th day offset.
 *
 *          The returned period_bounds structure contains the derived start and
 *          end date strings. Errors in schedule parsing (invalid N or D) are
 *          surfaced via exceptions, ensuring that invalid configurations do not
 *          propagate into statement generation.
 ******************************************************************************/
#ifndef _DATE_MANAGER_H_
#define _DATE_MANAGER_H_
#include <string>

namespace utility {
struct period_bounds
{
    std::string period_start;
    std::string period_end;
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
