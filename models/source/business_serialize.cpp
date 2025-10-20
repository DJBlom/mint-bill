
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

storage::database::sql_parameters serialize::business::package_data(const std::any& _data)
{
	storage::database::sql_parameters params{};
	data::business business_data{std::any_cast<data::business>(_data)};
	if (business_data.is_valid() == false)
	{
		syslog(LOG_CRIT, "BUSINESS_SERIALIZE: argument is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		params.emplace_back(business_data.get_name());
		params.emplace_back(business_data.get_address());
		params.emplace_back(business_data.get_area_code());
		params.emplace_back(business_data.get_town());
		params.emplace_back(business_data.get_cellphone());
		params.emplace_back(business_data.get_email());
	}

	return params;
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
