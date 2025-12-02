
#include <syslog.h>
#include <admin_serialize.h>

serialize::admin::~admin() {}

// storage::database::sql_parameters serialize::admin::package_data(const std::any& _data)
// {
// 	storage::database::sql_parameters params{};
// 	data::admin admin_data{std::any_cast<data::admin>(_data)};
// 	if (admin_data.is_valid() == false)
// 	{
// 		syslog(LOG_CRIT, "ADMIN_SERIALIZE: argument is not valid - "
// 				 "filename %s, line number %d", __FILE__, __LINE__);
// 	}
// 	else
// 	{
// 		params.emplace_back(admin_data.get_name());
// 		params.emplace_back(admin_data.get_bank());
// 		params.emplace_back(admin_data.get_branch_code());
// 		params.emplace_back(admin_data.get_account_number());
// 		params.emplace_back(admin_data.get_password());
// 		params.emplace_back(admin_data.get_client_message());
// 	}
//
// 	return params;
// }

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
