#include <date_manager.h>
#include <chrono>
#include <sstream>
#include <iomanip>




std::string utility::date_manager::current_date()
{
	std::ostringstream date_ss{""};
	const std::chrono::time_point now{std::chrono::system_clock::now()};
	const std::chrono::year_month_day date{
		std::chrono::year_month_day{
			floor<std::chrono::days>(
					std::chrono::zoned_time{std::chrono::current_zone(),
					std::chrono::system_clock::now()}.get_local_time()
					)
		}
	};
	date_ss << date.month() << "-" << date.day() << "-" << date.year();

	return date_ss.str();
}




utility::period_bounds utility::date_manager::compute_period_bounds(const std::string& _schedule)
{
	if (_schedule.empty() == true)
	{
		return period_bounds{};
	}

	int N = 0;
	int D = 0;

	// Parse "N,D"
	{
		std::size_t pos = _schedule.find(',');
		if (pos == std::string::npos)
		{
			throw std::runtime_error("Invalid schedule: expected N,D");
		}

		N = std::stoi(_schedule.substr(0, pos));
		D = std::stoi(_schedule.substr(pos + 1));

		if (N < 1 || N > 4)
		{
			throw std::runtime_error("Invalid N (must be in [1,4])");
		}

		if (D < 1 || D > 7)
		{
			throw std::runtime_error("Invalid D (must be in [1,7])");
		}
	}

	// Current LOCAL date
	std::chrono::system_clock::time_point now_sys =
		std::chrono::system_clock::now();

	std::chrono::zoned_time local_zone_time{
		std::chrono::current_zone(), now_sys
	};

	auto local_time = local_zone_time.get_local_time();
	std::chrono::local_days local_days =
		std::chrono::floor<std::chrono::days>(local_time);

	std::chrono::year_month_day today{ local_days };
	std::chrono::year  y = today.year();
	std::chrono::month m = today.month();
	std::chrono::day   d = today.day();

	auto to_sa = [](const std::chrono::year_month_day& ymd) -> std::string
	{
		std::stringstream ss;
		ss << static_cast<unsigned>(ymd.month()) << "-"
			<< static_cast<unsigned>(ymd.day())   << "-"
			<< static_cast<int>(ymd.year());
		return ss.str();
	};

	// ---------- N = 1 : weekly (not month-bound) ----------
	if (N == 1)
	{
		std::chrono::sys_days today_sys{ today };
		std::chrono::weekday today_wd{ today_sys };

		// Monday-based week
		int diff = (today_wd - std::chrono::Monday).count();
		if (diff < 0)
		{
			diff += 7;
		}

		std::chrono::sys_days monday_this_week =
			today_sys - std::chrono::days(diff);

		std::chrono::sys_days end_sys =
			monday_this_week + std::chrono::days(D - 1);

		std::chrono::year_month_day start_ymd{ monday_this_week };
		std::chrono::year_month_day end_ymd{ end_sys };

		period_bounds result{
			to_sa(start_ymd),
				to_sa(end_ymd)
		};

		return result;
	}

	// ---------- N = 2,3,4 : month-based ----------
	// First and last day of month
	std::chrono::year_month_day first_of_month{
		y, m, std::chrono::day{1}
	};
	std::chrono::sys_days first_days{ first_of_month };

	std::chrono::year_month_day_last last_ymdl{
		y, std::chrono::month_day_last{ m }
	};
	std::chrono::sys_days last_days{ last_ymdl };

	// ✅ FIX: Week 1 = first *Monday in the month*, not the week containing the 1st
	std::chrono::weekday first_wd{ first_days };
	int to_monday = (std::chrono::Monday - first_wd).count();
	if (to_monday < 0)
	{
		to_monday += 7;
	}

	// This is the first Monday *on or after* the 1st → first full week in the month
	std::chrono::sys_days monday_week1 =
		first_days + std::chrono::days(to_monday);

	// N-th week start (relative to that week1)
	std::chrono::sys_days weekN_start =
		monday_week1 + std::chrono::days(7 * (N - 1));

	// D-th day in that N-th week (Mon-based)
	std::chrono::sys_days raw_end_sys =
		weekN_start + std::chrono::days(D - 1);

	// Clamp to [first_of_month, last_of_month]
	if (raw_end_sys < first_days)
	{
		raw_end_sys = first_days;
	}
	else if (raw_end_sys > last_days)
	{
		raw_end_sys = last_days;
	}

	std::chrono::year_month_day start_ymd = first_of_month;
	std::chrono::year_month_day end_ymd{ raw_end_sys };

	period_bounds result{
		to_sa(start_ymd),
			to_sa(end_ymd)
	};

	return result;
}
