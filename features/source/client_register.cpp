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
        std::vector<std::unordered_map<std::string, std::string>> container;
        container = storage.retrieve(business_name);
        std::unordered_map<std::string, std::string> data = container.front();

        data::client client_data{};
        client_data.set_business_name(data.at("business name"));
        client_data.set_business_address(data.at("business address"));
        client_data.set_business_area_code(data.at("business area code"));
        client_data.set_business_town_name(data.at("business town"));
        client_data.set_cellphone_number(data.at("cellphone"));
        client_data.set_email(data.at("email"));
        client_data.set_vat_number(data.at("vat number"));
        client_data.set_statement_schedule(data.at("statement schedule"));

        return client_data;
}

bool feature::client::save(const data::client& data, const interface::storage& storage)
{
        bool saved{false};
        if (data.is_valid() == true)
        {
                saved = storage.update();
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

data::client feature::client::search(const std::string& business_name, const interface::storage& storage)
{
        data::client client_data{};
        if (storage.update() && !business_name.empty())
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
