/********************************************************
 * Contents: Business feature implementation
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#include <business_feature.h>
#include <iostream>

feature::business::~business()
{

}

void feature::business::load(data::business& data)
{
        data.set_name("T.M Engineering");
        data.set_address("Geelsterd 8");
        data.set_area_code("6536");
        data.set_town("george");
        data.set_cellphone("0832315944");
        data.set_email("odn@gmail.com");
        data.set_bank("Standard Bank Oudtshoorn");
        data.set_branch_code("050514");
        data.set_account_number("371613191");
        data.set_client_message("Thank you for your support.");
}

bool feature::business::save(data::business& data)
{
        bool saved{false};
        if (data.is_valid() == true)
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
        else
        {
        }

        return saved;
}
