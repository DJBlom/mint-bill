/********************************************************
 * Contents: Storage class implementation
 * Author: Dawid J. Blom
 * Date: November 25, 2024
 *
 * NOTE:
 *******************************************************/
#include <sql.h>

storage::sql::~sql()
{

}

std::vector<std::unordered_map<std::string, std::string>> storage::sql::retrieve(const std::string& name) const
{
        std::vector<std::unordered_map<std::string, std::string>> vector;
        if (name == "tme")
        {
                std::unordered_map<std::string, std::string> data = {
                        {"name", "T.M Engineering"},
                        {"address", "Geelsterd 8"},
                        {"area code", "6536"},
                        {"town", "George"},
                        {"cellphone", "0832315944"},
                        {"email", "odn@gmail.com"},
                        {"bank", "Standard Bank"},
                        {"branch code", "050514"},
                        {"account number", "371613191"},
                        {"client message", "Thank you for your support."}
                };
                vector.push_back(data);
        }
        else
        {
                std::unordered_map<std::string, std::string> data = {
                        {"name", "T.C Engineering & Hydraulics"},
                        {"address", "Rademeyer Straat 13"},
                        {"area code", "6536"},
                        {"town", "George"},
                        {"cellphone", "0832315944"},
                        {"email", "odn@gmail.com"},
                        {"bank", "Nedbank"},
                        {"branch code", "050514"},
                        {"account number", "371613191"},
                        {"client message", "Thank you for your support."}
                };
                vector.push_back(data);
        }

        return vector;
}

bool storage::sql::update() const
{
        bool updated{false};
        updated = true;
        return updated;
}

bool storage::sql::insert() const
{
        bool inserted{false};
        inserted = true;
        return inserted;
}
