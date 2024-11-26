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
        data::business business_data{};
        std::vector<std::unordered_map<std::string, std::string>> container;
        container = storage.retrieve();
        std::unordered_map<std::string, std::string> data = container.front();

        business_data.set_name(data.at("name"));
        business_data.set_address(data.at("address"));
        business_data.set_area_code(data.at("area code"));
        business_data.set_town(data.at("town"));
        business_data.set_cellphone(data.at("cellphone"));
        business_data.set_email(data.at("email"));
        business_data.set_bank(data.at("bank"));
        business_data.set_branch_code(data.at("branch code"));
        business_data.set_account_number(data.at("account number"));
        business_data.set_client_message(data.at("client message"));

        return business_data;
}

bool feature::business::save(const data::business& data, const interface::storage& storage)
{
        bool saved{false};
        if (data.is_valid() == true)
        {
                saved = storage.update();

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
