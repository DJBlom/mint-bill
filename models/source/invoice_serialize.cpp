
#include <limits>
#include <syslog.h>
#include <invoice_data.h>
#include <statement_data.h>
#include <invoice_serialize.h>


serialize::invoice::~invoice() {}

std::vector<std::any> serialize::invoice::extract_data(const storage::database::part::rows& _rows)
{
	std::vector<std::any> any_data{};
	if (_rows.empty() == true)
	{
		syslog(LOG_CRIT, "INVOICE_SERIALIZE: argument is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		for (const data::invoice& invoice_data : collect_values(_rows))
		{
			any_data.emplace_back(std::move(invoice_data));
		}
	}

	return any_data;
}

void serialize::invoice::set_schedule(const std::string& _schedule)
{
	(void) _schedule;
}

std::vector<data::invoice> serialize::invoice::collect_values(const storage::database::part::rows& _rows)
{
	std::vector<data::invoice> invoice_sql_data{};
	for (const storage::database::part::row& row : _rows)
	{
		int col_index{0};
		data::invoice data{};
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
				else if (col_index == DATA_FIELDS::INVOICE_NUMBER)
				{
					if constexpr (std::is_same_v<T, sqlite3_int64>)
					{
						data.set_id(std::to_string(arg));
					}
				}
				else if (col_index == DATA_FIELDS::ORDER_NUMBER)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						data.set_order_number(arg);
					}
				}
				else if (col_index == DATA_FIELDS::JOB_CARD_NUMBER)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						data.set_job_card_number(arg);
					}
				}
				else if (col_index == DATA_FIELDS::INVOICE_DATE)
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
				else if (col_index == DATA_FIELDS::MATERIAL_TOTAL)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						data.set_material_total(arg);
					}
				}
				else if (col_index == DATA_FIELDS::DESCRIPTION_TOTAL)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						data.set_description_total(arg);
					}
				}
				else if (col_index == DATA_FIELDS::GRAND_TOTAL)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						data.set_grand_total(arg);
					}
				}
			}, column_value);
			++col_index;
		}
		invoice_sql_data.emplace_back(data);
	}

	return invoice_sql_data;
}




// column
std::vector<data::column> serialize::labor::extract_data(const storage::database::part::rows& _rows)
{
	std::vector<data::column> column_data{};
	if (_rows.empty() == true)
	{
		syslog(LOG_CRIT, "COLUMN SERIALIZE: argument is not valid - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		column_data = std::move(collect_values(_rows));
	}

	return column_data;
}

std::vector<data::column> serialize::labor::collect_values(const storage::database::part::rows& _rows)
{
	std::vector<data::column> column_sql_data{};
	for (const storage::database::part::row& row : _rows)
	{
		int col_index{0};
		data::column data{};
		for (const storage::database::part::column_value& column_value : row)
		{
			std::visit([&](auto&& arg) {
				using T = std::decay_t<decltype(arg)>;
				if (col_index == DATA_FIELDS::QUANTITY)
				{
					if constexpr (std::is_same_v<T, sqlite3_int64>)
					{
						data.set_quantity(static_cast<int> (arg));
					}
				}
				else if (col_index == DATA_FIELDS::DESCRIPTION)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						data.set_description(arg);
					}
				}
				else if (col_index == DATA_FIELDS::AMOUNT)
				{
					if constexpr (std::is_same_v<T, double>)
					{
						data.set_amount(arg);
					}
				}
				else if (col_index == DATA_FIELDS::ROW_NUMBER)
				{
					if constexpr (std::is_same_v<T, sqlite3_int64>)
					{
						data.set_row_number(arg);
					}
				}
				else if (col_index == DATA_FIELDS::IS_DESCRIPTION)
				{
					if constexpr (std::is_same_v<T, sqlite3_int64>)
					{
						data.set_is_description(static_cast<int>(arg));
					}
				}
			}, column_value);
			++col_index;
		}

		column_sql_data.emplace_back(data);
	}

	return column_sql_data;
}
