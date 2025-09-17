/********************************************************
 * Contents: admin info controller implementation
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#include <admin_controller.h>
#include <iostream>
#include <string>


controller::admin::~admin()
{

}

std::any controller::admin::load(const std::string& _keyword)
{
	(void) _keyword;
        data::admin business_data;
	business_data.set_name("name");
	business_data.set_address("address");
	business_data.set_area_code("area code");
	business_data.set_town("town");
	business_data.set_cellphone("cellphone");
	business_data.set_email("odn@gmail.com");
	business_data.set_bank("bank");
	business_data.set_branch_code("branch code");
	business_data.set_account_number("account number");
	business_data.set_password("");
	business_data.set_client_message("client message");

        return business_data;
}

bool controller::admin::save(const std::any& _data)
{
        bool saved{false};
	data::admin data{std::any_cast<data::admin> (_data)};
        if (data.is_valid())
        {
                saved = true;

                std::cout << "Name: " << data.get_name() << std::endl;
                std::cout << "Address: " << data.get_address() << std::endl;
                std::cout << "Area Code: " << data.get_area_code() << std::endl;
                std::cout << "Town: " << data.get_town() << std::endl;
                std::cout << "Cellphone: " << data.get_cellphone() << std::endl;
                std::cout << "Email: " << data.get_email() << std::endl;
                std::cout << "Bank: " << data.get_bank() << std::endl;
                std::cout << "Branch Code: " << data.get_branch_code() << std::endl;
                std::cout << "Account Number: " << data.get_account_number() << std::endl;
                std::cout << "Client Message: " << data.get_client_message() << std::endl;
        }

        return saved;
}
