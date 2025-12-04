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
		serialize::client client_serialize{};
		storage::database::sql_parameters client_param = {_business_name};
		storage::database::sqlite database{this->database_file, this->database_password};
		client_data = std::move(std::any_cast<data::client> (
					client_serialize.extract_data(
						database.select(sql::query::client_select, client_param)
						)
					)
				);
        }

        return client_data;
}

bool model::client::save(const std::any& _data)
{
        bool saved{false};
	data::client client_data = std::any_cast<data::client>(_data);
        if (client_data.is_valid() == false)
	{
                syslog(LOG_CRIT, "CLIENT_MODEL: invalid argument - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
        {
		data::business business_data{client_data};
		serialize::client client_serialize{};
		serialize::business business_serialize{};
		storage::database::sql_parameters client_params{
			client_data.get_name(),
			client_data.get_vat_number(),
			client_data.get_statement_schedule()
		};
		storage::database::sql_parameters business_params{
			business_data.get_name(),
			business_data.get_address(),
			business_data.get_area_code(),
			business_data.get_town(),
			business_data.get_cellphone(),
			business_data.get_email()
		};
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
		else if (database.usert(sql::query::business_details_usert, business_params) == false)
		{
			syslog(LOG_CRIT, "CLIENT_MODEL: failed to execute sql query - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "CLIENT_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else if (database.usert(sql::query::client_usert, client_params) == false)
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
