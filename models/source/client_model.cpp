/********************************************************
 * Contents: Client register implementation
 * Author: Dawid J. Blom
 * Date: November 27, 2024
 *
 * NOTE:
 *******************************************************/
#include <client_model.h>
#include <client_data.h>
#include <syslog.h>
#include <iostream>


namespace sql {
namespace query {
	constexpr const char *business_details_usert{R"sql(
				INSERT INTO business_details (
					business_name, email_address, contact_number, street, area_code, town_name
				) VALUES (?, ?, ?, ?, ?, ?)
				ON CONFLICT(email_address) DO UPDATE SET
					business_name = excluded.business_name,
					email_address = excluded.email_address,
					contact_number = excluded.contact_number,
					street = excluded.street,
					area_code = excluded.area_code,
					town_name = excluded.town_name;)sql"};

	constexpr const char *client_usert{R"sql(
				INSERT INTO client (
					business_id,
					vat_number,
					statement_schedule
				) VALUES (
					(SELECT business_id FROM business_details WHERE email_address = ?),
					?,?
				)
				ON CONFLICT(business_id) DO UPDATE SET
					vat_number = excluded.vat_number,
					statement_schedule = excluded.statement_schedule;)sql"};

	// constexpr const char* select{R"sql(
	// 	    SELECT  bd.business_name,
	// 		    bd.email_address,
	// 		    bd.contact_number,
	// 		    bd.street,
	// 		    bd.area_code,
	// 		    bd.town_name,
	// 		    a.bank_name,
	// 		    a.branch_code,
	// 		    a.account_number,
	// 		    a.app_password,
	// 		    a.client_message
	// 	    FROM admin AS a
	// 	    JOIN business_details AS bd
	// 		ON a.business_id = bd.business_id
	// 	    WHERE a.business_id = ? AND bd.business_name = ?)sql"};
}
}

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
        if (!_business_name.empty())
        {
                client_data.set_business_name("Dummy");
                client_data.set_business_address("Geelsterd 8");
                client_data.set_business_area_code("05693");
                client_data.set_business_town_name("George");
                client_data.set_cellphone_number("0711422488");
                client_data.set_email("client@gmail.com");
                client_data.set_vat_number("425435");
                client_data.set_statement_schedule("4,4");
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
		storage::database::sqlite database{this->database_file, this->database_password};
		details details{2};
		details[PARAMETERS::DETAILS] = {
			data.get_business_name(),
			data.get_email(),
			data.get_cellphone_number(),
			data.get_business_address(),
			data.get_business_area_code(),
			data.get_business_town_name()};
		details[PARAMETERS::CLIENT] = {
			data.get_email(),
			data.get_vat_number(),
			data.get_statement_schedule()};
		if (database.usert(sql::query::business_details_usert, details[PARAMETERS::DETAILS]) == false)
		{
			syslog(LOG_CRIT, "CLIENT_MODEL: failed to execute sql query - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else if (database.usert(sql::query::client_usert, details[PARAMETERS::CLIENT]) == false)
		{
			syslog(LOG_CRIT, "CLIENT_MODEL: failed to execute sql query - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			saved = true;
		}
        }

        return saved;
}
