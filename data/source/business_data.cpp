/********************************************************
 * Contents: Business data implementation
 * Author: Dawid J. Blom
 * Date: November 16, 2024
 *
 * NOTE:
 *******************************************************/
#include <business_data.h>
#include <iostream>


bool data::business::is_valid()
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
        if (!business_name.empty())
        {
                set_flag(FLAG::NAME);
                std::lock_guard<std::mutex> guard(this->data);
                this->name = business_name;
        }
        else
        {
                clear_flag(FLAG::NAME);
        }
}

std::string data::business::get_name()
{
        return this->name;
}

void data::business::set_address(const std::string& street_address)
{
        if (!street_address.empty())
        {
                set_flag(FLAG::ADDRESS);
                std::lock_guard<std::mutex> guard(this->data);
                this->address = street_address;
        }
        else
        {
                clear_flag(FLAG::ADDRESS);
        }
}

std::string data::business::get_address()
{
        return this->address;
}

void data::business::set_area_code(const std::string& code)
{
        if (!code.empty())
        {
                set_flag(FLAG::AREA_CODE);
                std::lock_guard<std::mutex> guard(this->data);
                this->area_code = code;
        }
        else
        {
                clear_flag(FLAG::AREA_CODE);
        }
}

std::string data::business::get_area_code()
{
        return this->area_code;
}

void data::business::set_town(const std::string& business_town)
{
        if (!business_town.empty())
        {
                set_flag(FLAG::TOWN);
                std::lock_guard<std::mutex> guard(this->data);
                this->town = business_town;
        }
        else
        {
                clear_flag(FLAG::TOWN);
        }
}

std::string data::business::get_town()
{
        return this->town;
}

void data::business::set_cellphone(const std::string& cellphone_number)
{
        if (!cellphone_number.empty())
        {
                set_flag(FLAG::CELLPHONE);
                std::lock_guard<std::mutex> guard(this->data);
                this->cellphone = cellphone_number;
        }
        else
        {
                clear_flag(FLAG::CELLPHONE);
        }
}

std::string data::business::get_cellphone()
{
        return this->cellphone;
}

void data::business::set_email(const std::string& email_address)
{
        if (!email_address.empty())
        {
                set_flag(FLAG::EMAIL);
                std::lock_guard<std::mutex> guard(this->data);
                this->email = email_address;
        }
        else
        {
                clear_flag(FLAG::EMAIL);
        }
}

std::string data::business::get_email()
{
        return this->email;
}

void data::business::set_bank(const std::string& bank_name)
{
        if (!bank_name.empty())
        {
                set_flag(FLAG::BANK);
                std::lock_guard<std::mutex> guard(this->data);
                this->bank = bank_name;
        }
        else
        {
                clear_flag(FLAG::BANK);
        }
}

std::string data::business::get_bank()
{
        return this->bank;
}

void data::business::set_branch_code(const std::string& branch_num)
{
        if (!branch_num.empty())
        {
                set_flag(FLAG::BRANCH_CODE);
                std::lock_guard<std::mutex> guard(this->data);
                this->branch_code = branch_num;
        }
        else
        {
                clear_flag(FLAG::BRANCH_CODE);
        }
}

std::string data::business::get_branch_code()
{
        return this->branch_code;
}

void data::business::set_account_number(const std::string& account_num)
{
        if (!account_num.empty())
        {
                set_flag(FLAG::ACCOUNT_NUMBER);
                std::lock_guard<std::mutex> guard(this->data);
                this->account_number = account_num;
        }
        else
        {
                clear_flag(FLAG::ACCOUNT_NUMBER);
        }
}

std::string data::business::get_account_number()
{
        return this->account_number;
}

void data::business::set_client_message(const std::string& message)
{
        if (!message.empty())
        {
                set_flag(FLAG::CLIENT_MESSAGE);
                std::lock_guard<std::mutex> guard(this->data);
                this->client_message = message;
        }
        else
        {
                clear_flag(FLAG::CLIENT_MESSAGE);
        }
}

std::string data::business::get_client_message()
{
        return this->client_message;
}

bool data::business::check_flags()
{
        return ((this->flags & this->mask) == 0x03FF) ? true : false;
}

void data::business::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->data);
        this->flags |= static_cast<uint16_t>(1 << bit);
}

void data::business::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->data);
        this->flags |= static_cast<uint16_t>(0 << bit);
}
