/********************************************************
 * Contents: admin info model implementation
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#include <admin_model.h>
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

	constexpr const char* select{R"sql(
		    SELECT  bd.business_name,
			    bd.email_address,
			    bd.contact_number,
			    bd.street,
			    bd.area_code,
			    bd.town_name,
			    a.bank_name,
			    a.branch_code,
			    a.account_number,
			    a.app_password,
			    a.client_message
		    FROM admin AS a
		    JOIN business_details AS bd
			ON a.business_id = bd.business_id
		    WHERE a.business_id = ? AND bd.business_name = ?)sql"};
}
}

model::admin::admin(const std::string& _database_file, const std::string& _database_password)
	: database_file{_database_file}, database_password{_database_password}
{
}

model::admin::~admin()
{

}

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
		storage::database::sqlite database{this->database_file, this->database_password};
		std::vector<storage::database::param_values> query_argument = {this->business_id, _business_name};
		storage::database::part::rows rows{database.select(sql::query::select, query_argument)};
		admin_data = extract_data(rows);
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
		details details{package_data(data)};
		if (database.usert(sql::query::business_details_usert, details[PARAMETERS::DETAILS]) == false)
		{
			syslog(LOG_CRIT, "ADMIN_MODEL: failed to execute sql query - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else if (database.usert(sql::query::admin_usert, details[PARAMETERS::ADMIN]) == false)
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

data::admin model::admin::extract_data(const storage::database::part::rows& _rows)
{
	data::admin admin_data{};
	if (_rows.empty() == true)
	{
		syslog(LOG_CRIT, "ADMIN_MODEL: argument is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		std::vector<std::string> data{};
		for (const storage::database::part::row& row : _rows)
		{
			for (const storage::database::part::column_value& column_value : row)
			{
				data.emplace_back(std::visit([&admin_data] (auto&& arg) -> std::string {
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

		admin_data.set_name(data[DATA_FIELDS::NAME]);
		admin_data.set_email(data[DATA_FIELDS::EMAIL]);
		admin_data.set_cellphone(data[DATA_FIELDS::CELLPHONE]);
		admin_data.set_address(data[DATA_FIELDS::ADDRESS]);
		admin_data.set_area_code(data[DATA_FIELDS::AREA_CODE]);
		admin_data.set_town(data[DATA_FIELDS::TOWN]);
		admin_data.set_bank(data[DATA_FIELDS::BANK]);
		admin_data.set_branch_code(data[DATA_FIELDS::BRANCH_CODE]);
		admin_data.set_account_number(data[DATA_FIELDS::ACCOUNT_NUMBER]);
		admin_data.set_password(data[DATA_FIELDS::APP_PASSWORD]);
		admin_data.set_client_message(data[DATA_FIELDS::CLIENT_MESSAGE]);
	}

	return admin_data;
}

model::admin::details model::admin::package_data(const data::admin& _data)
{
	details details{2};
	if (_data.is_valid() == false)
	{
		syslog(LOG_CRIT, "ADMIN_MODEL: argument is not valid - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		details[PARAMETERS::DETAILS] = {
			this->business_id,
			_data.get_name(),
			_data.get_email(),
			_data.get_cellphone(),
			_data.get_address(),
			_data.get_area_code(),
			_data.get_town()};
		details[PARAMETERS::ADMIN] = {
			this->business_id,
			_data.get_bank(),
			_data.get_branch_code(),
			_data.get_account_number(),
			_data.get_password(),
			_data.get_client_message()};
	}

	return details;
}
