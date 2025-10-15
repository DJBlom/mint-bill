/********************************************************
 * Contents: Client register implementation
 * Author: Dawid J. Blom
 * Date: November 27, 2024
 *
 * NOTE:
 *******************************************************/
#include <syslog.h>
#include <iostream>
#include <client_data.h>
#include <client_model.h>
#include <client_serialize.h>
#include <business_serialize.h>


model::client::client(const std::string& _database_file, const std::string& _database_password)
	: database_file{_database_file}, database_password{_database_password}
{
}

model::client::~client()
{
}

std::any model::client::load(const std::string& _business_name)
{
        data::client client_data{};
        if (_business_name.empty() == true)
	{
                syslog(LOG_CRIT, "CLIENT_MODEL: invalid argument - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
        {
		storage::database::part::rows rows{};
		storage::database::sqlite database{this->database_file, this->database_password};
		std::vector<storage::database::param_values> query_argument = {_business_name};
		if (database.transaction("BEGIN IMMEDIATE;") == false)
		{
			syslog(LOG_CRIT, "CLIENT_MODEL: failed to begin transaction - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "CLIENT_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else
		{
			rows = database.select(sql::query::client_select, query_argument);
		}

		if (database.transaction("COMMIT;") == false)
		{
			syslog(LOG_CRIT, "CLIENT_MODEL: failed to commit transaction - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "CLIENT_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else
		{
			client_data = extract_data(rows);
		}
        }

        return client_data;
}

bool model::client::save(const std::any& _data)
{
        bool saved{false};
	data::client data = std::any_cast<data::client>(_data);
        if (data.is_valid() == false)
	{
                syslog(LOG_CRIT, "CLIENT_MODEL: invalid argument - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
        {
		details details{package_data(data)};
		storage::database::sqlite database{this->database_file, this->database_password};
		if (database.transaction("BEGIN IMMEDIATE;") == false)
		{
			syslog(LOG_CRIT, "CLIENT_MODEL: failed to begin transaction - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "CLIENT_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else if (database.usert(sql::query::business_details_usert, details[PARAMETERS::DETAILS]) == false)
		{
			syslog(LOG_CRIT, "CLIENT_MODEL: failed to execute sql query - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "CLIENT_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else if (database.usert(sql::query::client_usert, details[PARAMETERS::CLIENT]) == false)
		{
			syslog(LOG_CRIT, "CLIENT_MODEL: failed to execute sql query - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "CLIENT_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else if (database.transaction("COMMIT;") == false)
		{
			syslog(LOG_CRIT, "CLIENT_MODEL: failed to commit transaction - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "CLIENT_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else
		{
			saved = true;
		}
        }

        return saved;
}

data::client model::client::extract_data(const storage::database::part::rows& _rows)
{
	data::client client_data{};
	if (_rows.empty() == true)
	{
		syslog(LOG_CRIT, "CLIENT_MODEL: invalid argument - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		std::vector<std::string> data{};
		for (const storage::database::part::row& row : _rows)
		{
			for (const storage::database::part::column_value& column_value : row)
			{
				data.emplace_back(std::visit([&client_data] (auto&& arg) -> std::string {
					using T = std::decay_t<decltype(arg)>;
					std::string result{""};
					if constexpr (std::is_same_v<T, std::string>)
					{
						result = arg;
					}
					return result;
				}, column_value));
			}
		}

		client_data.set_name(data[DATA_FIELDS::NAME]);
		client_data.set_email(data[DATA_FIELDS::EMAIL]);
		client_data.set_address(data[DATA_FIELDS::ADDRESS]);
		client_data.set_area_code(data[DATA_FIELDS::AREA_CODE]);
		client_data.set_town(data[DATA_FIELDS::TOWN_NAME]);
		client_data.set_cellphone(data[DATA_FIELDS::CELLPHONE_NUMBER]);
		client_data.set_vat_number(data[DATA_FIELDS::VAT_NUMBER]);
		client_data.set_statement_schedule(data[DATA_FIELDS::STATEMENT_SCHEDULE]);
	}

	return client_data;
}

model::client::details model::client::package_data(const data::client& _data)
{
	details details{2};
	if (_data.is_valid() == false)
	{
		syslog(LOG_CRIT, "CLIENT_MODEL: argument is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		details[PARAMETERS::DETAILS] = {
			_data.get_name(),
			_data.get_email(),
			_data.get_cellphone(),
			_data.get_address(),
			_data.get_area_code(),
			_data.get_town()};
		details[PARAMETERS::CLIENT] = {
			_data.get_email(),
			_data.get_vat_number(),
			_data.get_statement_schedule()};
	}

	return details;
}
