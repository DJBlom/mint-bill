
#include <syslog.h>
#include <client_data.h>
#include <client_serialize.h>


serialize::client::~client() {}

std::any serialize::client::extract_data(const storage::database::part::rows& _rows)
{
	data::client client_data{};
	if (_rows.empty() == true)
	{
		syslog(LOG_CRIT, "CLIENT_SERIALIZE: argument is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		client_data = std::move(collect_values(_rows));
	}

	return client_data;
}

data::client serialize::client::collect_values(const storage::database::part::rows& _rows)
{
	data::client client_data{};
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
						client_data.set_name(arg);
					}
				}
				else if (col_index == DATA_FIELDS::ADDRESS)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						client_data.set_address(arg);
					}
				}
				else if (col_index == DATA_FIELDS::AREA_CODE)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						client_data.set_area_code(arg);
					}
				}
				else if (col_index == DATA_FIELDS::TOWN)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						client_data.set_town(arg);
					}
				}
				else if (col_index == DATA_FIELDS::CELLPHONE)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						client_data.set_cellphone(arg);
					}
				}
				else if (col_index == DATA_FIELDS::EMAIL)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						client_data.set_email(arg);
					}
				}
				else if (col_index == DATA_FIELDS::VAT_NUMBER)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						client_data.set_vat_number(arg);
					}
				}
				else if (col_index == DATA_FIELDS::STATEMENT_SCHEDULE)
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						client_data.set_statement_schedule(arg);
					}
				}
			}, column_value);
			++col_index;
		}
	}

	return client_data;
}
