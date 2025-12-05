/*******************************************************************************
 * @file admin_serialize.cpp
 *
 * @brief Implementation of the admin serialization logic.
 *
 * @details
 * This source file defines the behavior of `serialize::admin`, which is
 * responsible for translating low-level SQLite result sets into a single
 * `data::admin` instance.
 *
 * Core workflow:
 *  - `extract_data()` validates the incoming row set and delegates to
 *    `collect_values()` when data is present, logging failures via `syslog`.
 *  - `collect_values()` iterates over each row and column, using
 *    `std::visit` on the variant column type to assign string values to
 *    the appropriate fields in `data::admin` based on the `DATA_FIELDS`
 *    enum ordering.
 *
 * This keeps SQL/schema details localized to the serialization layer and
 * allows higher-level models and UI code to work with domain objects
 * instead of raw database rows.
 *******************************************************************************/
#include <syslog.h>
#include <admin_serialize.h>

serialize::admin::~admin() {}

std::any serialize::admin::extract_data(const storage::database::part::rows& _rows)
{
	data::admin admin_data{};
	if (_rows.empty() == true)
	{
		syslog(LOG_CRIT, "ADMIN_SERIALIZE: argument is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		admin_data = std::move(collect_values(_rows));
	}

	return admin_data;
}

data::admin serialize::admin::collect_values(const storage::database::part::rows& _rows)
{
	data::admin admin_data{};
	for (const storage::database::part::row& row : _rows)
	{
		int col_index{0};
		for (const storage::database::part::column_value& column_value : row)
		{
			std::visit([&] (auto&& arg) {
				using T = std::decay_t<decltype(arg)>;
				if (col_index == DATA_FIELDS::NAME)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						admin_data.set_name(arg);
					}
				}
				else if (col_index == DATA_FIELDS::ADDRESS)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						admin_data.set_address(arg);
					}
				}
				else if (col_index == DATA_FIELDS::AREA_CODE)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						admin_data.set_area_code(arg);
					}
				}
				else if (col_index == DATA_FIELDS::TOWN)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						admin_data.set_town(arg);
					}
				}
				else if (col_index == DATA_FIELDS::CELLPHONE)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						admin_data.set_cellphone(arg);
					}
				}
				else if (col_index == DATA_FIELDS::EMAIL)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						admin_data.set_email(arg);
					}
				}
				else if (col_index == DATA_FIELDS::BANK)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						admin_data.set_bank(arg);
					}
				}
				else if (col_index == DATA_FIELDS::BRANCH_CODE)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						admin_data.set_branch_code(arg);
					}
				}
				else if (col_index == DATA_FIELDS::ACCOUNT_NUMBER)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						admin_data.set_account_number(arg);
					}
				}
				else if (col_index == DATA_FIELDS::APP_PASSWORD)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						admin_data.set_password(arg);
					}
				}
				else if (col_index == DATA_FIELDS::CLIENT_MESSAGE)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						admin_data.set_client_message(arg);
					}
				}
			}, column_value);
			++col_index;
		}
	}

	return admin_data;
}
