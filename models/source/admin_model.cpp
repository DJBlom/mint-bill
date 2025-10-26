/********************************************************
 * Contents: admin info model implementation
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#include <string>
#include <syslog.h>
#include <admin_model.h>
#include <admin_serialize.h>
#include <business_serialize.h>


model::admin::admin(const std::string& _database_file, const std::string& _database_password)
	: database_file{_database_file}, database_password{_database_password} {}

model::admin::~admin() {}

std::any model::admin::load(const std::string& _business_name)
{
        data::admin admin_data;
	if (_business_name.empty() == true)
	{
                syslog(LOG_CRIT, "ADMIN_MODEL: invalid argument - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		serialize::admin admin_serialize{};
		storage::database::sql_parameters admin_param = {_business_name};
		storage::database::sqlite database{this->database_file, this->database_password};
		admin_data = std::move(std::any_cast<data::admin> (
					admin_serialize.extract_data(
						database.select(sql::query::admin_select, admin_param)
						)
					)
				);
	}

        return admin_data;
}

bool model::admin::save(const std::any& _data)
{
        bool saved{false};
	data::admin admin_data{std::any_cast<data::admin> (_data)};
        if (admin_data.is_valid() == false)
	{
                syslog(LOG_CRIT, "ADMIN_MODEL: data is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
        {
		serialize::admin admin_serialize{};
		data::business business_data{admin_data};
		serialize::business business_serialize{};
		storage::database::sql_parameters admin_params{admin_serialize.package_data(admin_data)};
		storage::database::sql_parameters business_params{business_serialize.package_data(business_data)};
		storage::database::sqlite database{this->database_file, this->database_password};
		if (database.transaction("BEGIN IMMEDIATE;") == false)
		{
			syslog(LOG_CRIT, "ADMIN_MODEL: failed to begin transaction - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "ADMIN_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else if (database.usert(sql::query::business_details_usert, business_params) == false)
		{
			syslog(LOG_CRIT, "ADMIN_MODEL: failed to execute sql query - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "ADMIN_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else if (database.usert(sql::query::admin_usert, admin_params) == false)
		{
			syslog(LOG_CRIT, "ADMIN_MODEL: failed to execute sql query - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "ADMIN_MODEL: failed to rollback - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
		}
		else if (database.transaction("COMMIT;") == false)
		{
			syslog(LOG_CRIT, "ADMIN_MODEL: failed to commit transaction - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			if (database.transaction("ROLLBACK;") == false)
			{
				syslog(LOG_CRIT, "ADMIN_MODEL: failed to rollback - "
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
