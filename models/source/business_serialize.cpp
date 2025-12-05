/*******************************************************************************
 * @file business_serialize.cpp
 *
 * @brief Implementation of the business serialization logic.
 *
 * @details
 * This source file defines the behavior of `serialize::business`, which is
 * responsible for translating low-level SQLite result sets into a strongly
 * typed `data::business` instance.
 *
 * Core workflow:
 *  - `extract_data()` validates the incoming row set and, when non-empty,
 *    calls `collect_values()` to extract ordered string values. It logs
 *    failures via `syslog` when no rows or no values are present.
 *  - `collect_values()` iterates through all rows and columns, using
 *    `std::visit` on the variant column type and collecting string
 *    representations of each field in the order defined by `DATA_FIELDS`.
 *  - The resulting vector is then mapped into the fields of `data::business`
 *    (name, address, area code, town, cellphone, email).
 *
 * By isolating this mapping in a dedicated component, the rest of the
 * application can work with typed business objects instead of dealing with
 * database-specific structures directly.
 *******************************************************************************/
#include <syslog.h>
#include <business_data.h>
#include <business_serialize.h>


serialize::business::~business() {}

std::any serialize::business::extract_data(const storage::database::part::rows& _rows)
{
	(void) _rows;
	data::business business_data{};
	if (_rows.empty() == true)
	{
		syslog(LOG_CRIT, "BUSINESS_SERIALIZE: argument is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		std::vector<std::string> business_sql_data{collect_values(_rows)};
		if (business_sql_data.empty() == true)
		{
			syslog(LOG_CRIT, "BUSINESS_SERIALIZE: business_sql_data collected are empty - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			business_data.set_name(business_sql_data[DATA_FIELDS::NAME]);
			business_data.set_address(business_sql_data[DATA_FIELDS::ADDRESS]);
			business_data.set_area_code(business_sql_data[DATA_FIELDS::AREA_CODE]);
			business_data.set_town(business_sql_data[DATA_FIELDS::TOWN]);
			business_data.set_cellphone(business_sql_data[DATA_FIELDS::CELLPHONE]);
			business_data.set_email(business_sql_data[DATA_FIELDS::EMAIL]);
		}
	}

	return business_data;
}

std::vector<std::string> serialize::business::collect_values(const storage::database::part::rows& _rows)
{
	std::vector<std::string> business_sql_data{};
	for (const storage::database::part::row& row : _rows)
	{
		for (const storage::database::part::column_value& column_value : row)
		{
			business_sql_data.emplace_back(std::visit([] (auto&& value) -> std::string {
				using T = std::decay_t<decltype(value)>;
				std::string result{""};
				if constexpr (std::is_same_v<T, std::string>)
				{
					result = value;
				}

				return result;
			}, column_value));
		}
	}

	return business_sql_data;
}
