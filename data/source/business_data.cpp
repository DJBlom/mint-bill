/********************************************************
 * Contents: Business data implementation
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#include <business_data.h>
#include <regex>

namespace upper_bound {
        constexpr std::uint8_t string_length{50};
        constexpr std::uint8_t message_length{100};
}

data::business::business(const business& new_data)
        : name{new_data.name}, address{new_data.address}, area_code{new_data.area_code},
          town{new_data.town}, cellphone{new_data.cellphone}, email{new_data.email},
          bank{new_data.bank}, branch_code{new_data.branch_code}, account_number{new_data.account_number},
          client_message{new_data.client_message}, flags{new_data.flags}, business_data{}, mask{new_data.mask}
{
}

data::business::business(business&& new_data)
        : name{new_data.name}, address{new_data.address}, area_code{new_data.area_code},
          town{new_data.town}, cellphone{new_data.cellphone}, email{new_data.email},
          bank{new_data.bank}, branch_code{new_data.branch_code}, account_number{new_data.account_number},
          client_message{new_data.client_message}, flags{new_data.flags}, business_data{}, mask{new_data.mask}
{
        new_data.name.clear();
        new_data.address.clear();
        new_data.area_code.clear();
        new_data.town.clear();
        new_data.cellphone.clear();
        new_data.email.clear();
        new_data.bank.clear();
        new_data.branch_code.clear();
        new_data.account_number.clear();
        new_data.client_message.clear();
        new_data.flags = 0;
        new_data.mask = 0x0;
}

data::business& data::business::operator= (const business& new_data)
{
        business temp {new_data};
        std::swap(temp, *this);

        return *this;
}

data::business& data::business::operator= (business&& new_data)
{
        std::swap(name, new_data.name);
        std::swap(address, new_data.address);
        std::swap(area_code, new_data.area_code);
        std::swap(town, new_data.town);
        std::swap(cellphone, new_data.cellphone);
        std::swap(email, new_data.email);
        std::swap(bank, new_data.bank);
        std::swap(branch_code, new_data.branch_code);
        std::swap(account_number, new_data.account_number);
        std::swap(client_message, new_data.client_message);
        std::swap(flags, new_data.flags);
        std::swap(mask, new_data.mask);

        return *this;
}

bool data::business::is_valid() const
{
        bool is_valid{false};
        if (this->check_flags() == true)
        {
                is_valid = true;
        }

        return is_valid;
}

void data::business::set_name(const std::string& business_name)
{
        if (!business_name.empty() && (business_name.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::NAME);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->name = business_name;
        }
        else
        {
                clear_flag(FLAG::NAME);
        }
}

std::string data::business::get_name() const
{
        return this->name;
}

void data::business::set_address(const std::string& street_address)
{
        if (!street_address.empty() && (street_address.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::ADDRESS);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->address = street_address;
        }
        else
        {
                clear_flag(FLAG::ADDRESS);
        }
}

std::string data::business::get_address() const
{
        return this->address;
}

void data::business::set_area_code(const std::string& code)
{
        if (!code.empty() && (code.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::AREA_CODE);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->area_code = code;
        }
        else
        {
                clear_flag(FLAG::AREA_CODE);
        }
}

std::string data::business::get_area_code() const
{
        return this->area_code;
}

void data::business::set_town(const std::string& business_town)
{
        if (!business_town.empty() && (business_town.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::TOWN);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->town = business_town;
        }
        else
        {
                clear_flag(FLAG::TOWN);
        }
}

std::string data::business::get_town() const
{
        return this->town;
}

void data::business::set_cellphone(const std::string& cellphone_number)
{
        if (!cellphone_number.empty() && (cellphone_number.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::CELLPHONE);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->cellphone = cellphone_number;
        }
        else
        {
                clear_flag(FLAG::CELLPHONE);
        }
}

std::string data::business::get_cellphone() const
{
        return this->cellphone;
}

void data::business::set_email(const std::string& email_address)
{
        std::regex email_regex(R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");
        bool correct_email_format{std::regex_search(email_address, email_regex)};
        if (correct_email_format && (email_address.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::EMAIL);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->email = email_address;
        }
        else
        {
                clear_flag(FLAG::EMAIL);
        }
}

std::string data::business::get_email() const
{
        return this->email;
}

void data::business::set_bank(const std::string& bank_name)
{
        if (!bank_name.empty() && (bank_name.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::BANK);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->bank = bank_name;
        }
        else
        {
                clear_flag(FLAG::BANK);
        }
}

std::string data::business::get_bank() const
{
        return this->bank;
}

void data::business::set_branch_code(const std::string& branch_num)
{
        if (!branch_num.empty() && (branch_num.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::BRANCH_CODE);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->branch_code = branch_num;
        }
        else
        {
                clear_flag(FLAG::BRANCH_CODE);
        }
}

std::string data::business::get_branch_code() const
{
        return this->branch_code;
}

void data::business::set_account_number(const std::string& account_num)
{
        if (!account_num.empty() && (account_num.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::ACCOUNT_NUMBER);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->account_number = account_num;
        }
        else
        {
                clear_flag(FLAG::ACCOUNT_NUMBER);
        }
}

std::string data::business::get_account_number() const
{
        return this->account_number;
}

void data::business::set_client_message(const std::string& message)
{
        if (!message.empty() && (message.length() <= upper_bound::message_length))
        {
                set_flag(FLAG::CLIENT_MESSAGE);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->client_message = message;
        }
        else
        {
                clear_flag(FLAG::CLIENT_MESSAGE);
        }
}

std::string data::business::get_client_message() const
{
        return this->client_message;
}

bool data::business::check_flags() const
{
        return ((this->flags & this->mask) == 0x03FF) ? true : false;
}

void data::business::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->business_data);
        this->flags |= static_cast<std::uint16_t>(1 << bit);
}

void data::business::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->business_data);
        this->flags |= static_cast<std::uint16_t>(0 << bit);
}
