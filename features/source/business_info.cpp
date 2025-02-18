/********************************************************
 * Contents: Business info feature implementation
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#include <business_info.h>
#include <iostream>
#include <string>


feature::business::~business()
{

}

data::business feature::business::load(const interface::storage& storage)
{
        data::business business_data;
        if (storage.is_open())
        {
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
        }

        return business_data;
}

bool feature::business::save(const data::business& data, const interface::storage& storage)
{
        bool saved{false};
        if (data.is_valid() && storage.is_open())
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
