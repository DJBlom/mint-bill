/********************************************************
 * Contents: admin info model implementation
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#include <admin_model.h>
#include <sqlite.h>
#include <syslog.h>
#include <string>


namespace sql {
namespace query {
	constexpr const char *business_details_usert{R"sql(
				INSERT INTO business_details (
				    business_id, business_name, email_address, contact_number, street, area_code, town_name
				) VALUES (?, ?, ?, ?, ?, ?, ?)
				ON CONFLICT(business_id) DO UPDATE SET
				    business_name  = excluded.business_name,
				    email_address  = excluded.email_address,
				    contact_number = excluded.contact_number,
				    street         = excluded.street,
				    area_code      = excluded.area_code,
				    town_name      = excluded.town_name;)sql"};

	constexpr const char *admin_usert{R"sql(
			INSERT INTO admin (
					business_id, bank_name, branch_code, account_number, app_password, client_message
				) VALUES (?, ?, ?, ?, ?, ?)
			ON CONFLICT(business_id) DO UPDATE SET
			bank_name = excluded.bank_name,
				       branch_code = excluded.branch_code,
				       account_number = excluded.account_number,
				       app_password = excluded.app_password,
				       client_message = excluded.client_message;)sql"};

	constexpr const char *select{R"sql(
		SELECT  bd.business_id,
			bd.business_name,
			bd.email_address,
			bd.contact_number,
			bd.street,
			bd.area_code,
			bd.town_name,
			a.bank_name,
			a.branch_code,
			a.account_number,
			a.app_password,
				FROM admin AS a
				JOIN business_details AS bd
				ON a.business_id = bd.business_id;)sql"};
}
}

model::admin::admin(const std::string& _database_file, const std::string& _database_password)
	: database_file{_database_file}, database_password{_database_password}
{
}

model::admin::~admin()
{

}

std::any model::admin::load(const std::string& _keyword)
{
        data::admin admin_data;
	if (_keyword.empty() == true)
	{
                syslog(LOG_CRIT, "ADMIN_MODEL: invalid argument - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		admin_data.set_name("name");
		admin_data.set_address("address");
		admin_data.set_area_code("area code");
		admin_data.set_town("town");
		admin_data.set_cellphone("cellphone");
		admin_data.set_email("odn@gmail.com");
		admin_data.set_bank("bank");
		admin_data.set_branch_code("branch code");
		admin_data.set_account_number("account number");
		admin_data.set_password("");
		admin_data.set_client_message("client message");
	}

        return admin_data;
}

bool model::admin::save(const std::any& _data)
{
        bool saved{false};
	data::admin data{std::any_cast<data::admin> (_data)};
        if (data.is_valid() == false)
	{
                syslog(LOG_CRIT, "ADMIN_MODEL: data is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
        {
		storage::database::sqlite database{this->database_file, this->database_password};
		std::vector<storage::database::param_values> business_details_arguments = {
			1LL,
			data.get_name(),
			data.get_email(),
			data.get_cellphone(),
			data.get_address(),
			data.get_area_code(),
			data.get_town()
		};
		std::vector<storage::database::param_values> admin_arguments = {
			1LL,
			data.get_bank(),
			data.get_branch_code(),
			data.get_account_number(),
			data.get_password(),
			data.get_client_message()
		};

		if (database.usert(sql::query::business_details_usert, business_details_arguments) == false)
		{
			syslog(LOG_CRIT, "ADMIN_MODEL: failed to execute sql query - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else if (database.usert(sql::query::admin_usert, admin_arguments) == false)
		{
			syslog(LOG_CRIT, "ADMIN_MODEL: failed to execute sql query - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			saved = true;
		}
        }

        return saved;
}
