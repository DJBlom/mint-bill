/********************************************************
 * Contents: Client register implementation
 * Author: Dawid J. Blom
 * Date: November 27, 2024
 *
 * NOTE:
 *******************************************************/
#include <client_register.h>
#include <client_data.h>
#include <iostream>


feature::client::~client()
{
}

data::client feature::client::load(const std::string& business_name, const interface::storage& storage)
{
        data::client client_data{};
        if (storage.is_open() && !business_name.empty())
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

bool feature::client::save(const data::client& data, const interface::storage& storage)
{
        bool saved{false};
        if (data.is_valid() && storage.is_open())
        {
                saved = true;
                std::cout << "Business Name: " << data.get_business_name() << std::endl;
                std::cout << "Business Address: " << data.get_business_address() << std::endl;
                std::cout << "Business Area Code: " << data.get_business_area_code() << std::endl;
                std::cout << "Business Town: " << data.get_business_town_name() << std::endl;
                std::cout << "Cellphone: " << data.get_cellphone_number() << std::endl;
                //std::cout << "Email: " << data.get_email() << std::endl;
                std::cout << "Vat Number: " << data.get_vat_number() << std::endl;
                std::cout << "Statement Schedule: " << data.get_statement_schedule() << std::endl;
        }

        return saved;
}

data::client feature::client::search(const std::string& business_name, const interface::storage& storage)
{
        data::client client_data{};
        if (storage.is_open() && !business_name.empty())
        {
                client_data.set_business_name("Dummy Business");
                client_data.set_business_address("Bloem Straat 7");
                client_data.set_business_area_code("5432");
                client_data.set_business_town_name("George");
                client_data.set_cellphone_number("0711422488");
                client_data.set_email("client@gmail.com");
                client_data.set_vat_number("49204");
                client_data.set_statement_schedule("4,4");
        }

        return client_data;
}
