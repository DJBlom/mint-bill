/********************************************************
 * Contents: Client register implementation
 * Author: Dawid J. Blom
 * Date: November 27, 2024
 *
 * NOTE:
 *******************************************************/
#include <client_model.h>
#include <client_data.h>
#include <iostream>


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
        if (data.is_valid())
        {
                saved = true;
                std::cout << "Business Name: " << data.get_business_name() << std::endl;
                std::cout << "Business Address: " << data.get_business_address() << std::endl;
                std::cout << "Business Area Code: " << data.get_business_area_code() << std::endl;
                std::cout << "Business Town: " << data.get_business_town_name() << std::endl;
                std::cout << "Cellphone: " << data.get_cellphone_number() << std::endl;
                std::cout << "Email: " << data.get_email() << std::endl;
                std::cout << "Vat Number: " << data.get_vat_number() << std::endl;
                std::cout << "Statement Schedule: " << data.get_statement_schedule() << std::endl;
        }

        return saved;
}
