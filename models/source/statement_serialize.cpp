/*******************************************************************************
 * @file statement_serialize.cpp
 *
 * @brief Implementation of the statement model and its serialization logic.
 *
 * @details
 * This file provides:
 *
 *   1. Implementation of serialize::statement
 *      - Converts database query results into std::vector<std::any> containing
 *        data::statement objects.
 *      - Uses collect_values() to map variant SQL column values to strongly
 *        typed data::statement fields using the DATA_FIELDS enum.
 *      - Logs critical errors via syslog() when invalid or empty result sets
 *        are encountered.
 *
 *   2. collect_values()
 *      - Iterates row-by-row through storage::database::part::rows.
 *      - Uses std::visit to safely match variant types to expected fields.
 *      - Populates domain objects by calling the appropriate setters
 *        (set_name, set_id, set_period_start, set_period_end, etc.).
 *
 * Error Handling:
 *   - Empty or malformed result sets immediately generate a syslog(LOG_CRIT)
 *     message including file and line information.
 *
 * The serializer is used by model::statement to transform raw SQL query output
 * into strongly typed domain objects for printing, emailing, or further processing.
 *******************************************************************************/
#include <limits>
#include <syslog.h>
#include <statement_serialize.h>

serialize::statement::~statement() {}

std::vector<std::any> serialize::statement::extract_data(const storage::database::part::rows& _rows)
{
	std::vector<std::any> any_data{};
	if (_rows.empty() == true)
	{
		syslog(LOG_CRIT, "STATEMENT_SERIALIZE: argument is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		for (const data::statement& stmt_data : collect_values(_rows))
		{
			any_data.emplace_back(std::move(stmt_data));
		}
	}

	return any_data;
}

std::vector<data::statement> serialize::statement::collect_values(const storage::database::part::rows& _rows)
{
	std::vector<data::statement> statement_sql_data{};
	for (const storage::database::part::row& row : _rows)
	{
		int col_index{0};
		data::statement data{};
		for (const storage::database::part::column_value& column_value : row)
		{
			std::visit([&](auto&& arg) {
				using T = std::decay_t<decltype(arg)>;
				if (col_index == DATA_FIELDS::BUSINESS_NAME)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						data.set_name(arg);
					}
				}
				else if (col_index == DATA_FIELDS::STATEMENT_ID)
				{
					if constexpr (std::is_same_v<T, sqlite3_int64>)
					{
						data.set_id(std::to_string(arg));
					}
				}
				else if (col_index == DATA_FIELDS::PERIOD_START)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						data.set_period_start(arg);
					}
				}
				else if (col_index == DATA_FIELDS::PERIOD_END)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						data.set_period_end(arg);
					}
				}
				else if (col_index == DATA_FIELDS::DATE)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						data.set_date(arg);
					}
				}
				else if (col_index == DATA_FIELDS::PAID_STATUS)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						data.set_paid_status(arg);
					}
				}
			}, column_value);
			++col_index;
		}

		statement_sql_data.emplace_back(data);
	}

	return statement_sql_data;
}
