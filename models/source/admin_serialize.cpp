
#include <syslog.h>
#include <admin_data.h>
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
		std::vector<std::string> admin_sql_data{collect_values(_rows)};
		if (admin_sql_data.empty() == true)
		{
			syslog(LOG_CRIT, "ADMIN_SERIALIZE: admin_sql_data collected are empty - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			admin_data.set_name(admin_sql_data[DATA_FIELDS::NAME]);
			admin_data.set_address(admin_sql_data[DATA_FIELDS::ADDRESS]);
			admin_data.set_area_code(admin_sql_data[DATA_FIELDS::AREA_CODE]);
			admin_data.set_town(admin_sql_data[DATA_FIELDS::TOWN]);
			admin_data.set_cellphone(admin_sql_data[DATA_FIELDS::CELLPHONE]);
			admin_data.set_email(admin_sql_data[DATA_FIELDS::EMAIL]);
			admin_data.set_bank(admin_sql_data[DATA_FIELDS::BANK]);
			admin_data.set_branch_code(admin_sql_data[DATA_FIELDS::BRANCH_CODE]);
			admin_data.set_account_number(admin_sql_data[DATA_FIELDS::ACCOUNT_NUMBER]);
			admin_data.set_password(admin_sql_data[DATA_FIELDS::APP_PASSWORD]);
			admin_data.set_client_message(admin_sql_data[DATA_FIELDS::CLIENT_MESSAGE]);
		}
	}

	return admin_data;
}

storage::database::sql_parameters serialize::admin::package_data(const std::any& _data)
{
	storage::database::sql_parameters params{};
	data::admin admin_data{std::any_cast<data::admin>(_data)};
	if (admin_data.is_valid() == false)
	{
		syslog(LOG_CRIT, "ADMIN_SERIALIZE: argument is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		params.emplace_back(admin_data.get_name());
		params.emplace_back(admin_data.get_bank());
		params.emplace_back(admin_data.get_branch_code());
		params.emplace_back(admin_data.get_account_number());
		params.emplace_back(admin_data.get_password());
		params.emplace_back(admin_data.get_client_message());
	}

	return params;
}

std::vector<std::string> serialize::admin::collect_values(const storage::database::part::rows& _rows)
{
	std::vector<std::string> admin_sql_data{};
	for (const storage::database::part::row& row : _rows)
	{
		for (const storage::database::part::column_value& column_value : row)
		{
			admin_sql_data.emplace_back(std::visit([] (auto&& value) -> std::string {
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

	return admin_sql_data;
}
