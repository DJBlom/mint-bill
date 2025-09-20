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


model::admin::admin(const std::string& _path_to_database_file, const std::string& _database_password)
	: path_to_database_file{_path_to_database_file}, database_password{_database_password}
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
        if (data.is_valid())
        {
                saved = true;

        }

        return saved;
}
