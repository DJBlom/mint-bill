/********************************************************
 * Contents: Client SQL Mock class implementation
 * Author: Dawid J. Blom
 * Date: November 27, 2024
 *
 * NOTE:
 *******************************************************/
#include <client_sql_mock.h>

my_mock::client_sql::~client_sql()
{
}

std::vector<std::unordered_map<std::string, std::string>>
my_mock::client_sql::retrieve(const std::string& business_name) const
{
        std::vector<std::unordered_map<std::string, std::string>> vector;
        if (!business_name.empty())
        {
                std::unordered_map<std::string, std::string> data = {
                        {"business name", "client business"},
                        {"business address", "doorn street 15"},
                        {"business area code", "6536"},
                        {"business town", "George"},
                        {"cellphone", "0711422488"},
                        {"email", "client@gmail.com"},
                        {"vat number", "343245"},
                        {"statement schedule", "4,4"},
                };
                vector.push_back(data);
        }

        return vector;
}

bool my_mock::client_sql::update() const
{
        return true;
}

bool my_mock::client_sql::insert() const
{
        return true;
}
