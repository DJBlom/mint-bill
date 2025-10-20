
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
		std::vector<std::string> client_sql_data{collect_values(_rows)};
		if (client_sql_data.empty() == true)
		{
			syslog(LOG_CRIT, "CLIENT_SERIALIZE: client_sql_data collected are empty - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			client_data.set_name(client_sql_data[DATA_FIELDS::NAME]);
			client_data.set_address(client_sql_data[DATA_FIELDS::ADDRESS]);
			client_data.set_area_code(client_sql_data[DATA_FIELDS::AREA_CODE]);
			client_data.set_town(client_sql_data[DATA_FIELDS::TOWN]);
			client_data.set_cellphone(client_sql_data[DATA_FIELDS::CELLPHONE]);
			client_data.set_email(client_sql_data[DATA_FIELDS::EMAIL]);
			client_data.set_vat_number(client_sql_data[DATA_FIELDS::VAT_NUMBER]);
			client_data.set_statement_schedule(client_sql_data[DATA_FIELDS::STATEMENT_SCHEDULE]);
		}
	}

	return client_data;
}

storage::database::sql_parameters serialize::client::package_data(const std::any& _data)
{
	storage::database::sql_parameters params{};
	data::client client_data{std::any_cast<data::client>(_data)};
	if (client_data.is_valid() == false)
	{
		syslog(LOG_CRIT, "CLIENT_SERIALIZE: argument is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		params.emplace_back(client_data.get_name());
		params.emplace_back(client_data.get_address());
		params.emplace_back(client_data.get_area_code());
		params.emplace_back(client_data.get_town());
		params.emplace_back(client_data.get_cellphone());
		params.emplace_back(client_data.get_email());
		params.emplace_back(client_data.get_vat_number());
		params.emplace_back(client_data.get_statement_schedule());
	}

	return params;
}

std::vector<std::string> serialize::client::collect_values(const storage::database::part::rows& _rows)
{
	std::vector<std::string> client_sql_data{};
	for (const storage::database::part::row& row : _rows)
	{
		for (const storage::database::part::column_value& column_value : row)
		{
			client_sql_data.emplace_back(std::visit([] (auto&& value) -> std::string {
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

	return client_sql_data;
}
