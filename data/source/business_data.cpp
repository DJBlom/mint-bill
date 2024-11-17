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
                set_flag(0);
                std::lock_guard<std::mutex> guard(this->data);
                this->name = business_name;
        }
        else
        {
                clear_flag(0);
        }
}

std::string data::business::get_name()
{
        std::lock_guard<std::mutex> guard(this->data);
        return this->name;
}

void data::business::set_address(const std::string& street_address)
{
        if (!street_address.empty())
        {
                set_flag(1);
                std::lock_guard<std::mutex> guard(this->data);
                this->address = street_address;
        }
        else
        {
                clear_flag(1);
        }
}

std::string data::business::get_address()
{
        std::lock_guard<std::mutex> guard(this->data);
        return this->address;
}

void data::business::set_town(const std::string& business_town)
{
        if (!business_town.empty())
        {
                set_flag(2);
                std::lock_guard<std::mutex> guard(this->data);
                this->town = business_town;
        }
        else
        {
                clear_flag(2);
        }
}

std::string data::business::get_town()
{
        std::lock_guard<std::mutex> guard(this->data);
        return this->town;
}

void data::business::set_cellphone(const std::string& cellphone_number)
{
        if (!cellphone_number.empty())
        {
                set_flag(3);
                std::lock_guard<std::mutex> guard(this->data);
                this->cellphone = cellphone_number;
        }
        else
        {
                clear_flag(3);
        }
}

std::string data::business::get_cellphone()
{
        std::lock_guard<std::mutex> guard(this->data);
        return this->cellphone;
}

void data::business::set_email(const std::string& email_address)
{
        if (!email_address.empty())
        {
                set_flag(4);
                std::lock_guard<std::mutex> guard(this->data);
                this->email = email_address;
        }
        else
        {
                clear_flag(4);
        }
}

std::string data::business::get_email()
{
        std::lock_guard<std::mutex> guard(this->data);
        return this->email;
}

void data::business::set_bank(const std::string& bank_name)
{
        if (!bank_name.empty())
        {
                set_flag(5);
                std::lock_guard<std::mutex> guard(this->data);
                this->bank = bank_name;
        }
        else
        {
                clear_flag(5);
        }
}

std::string data::business::get_bank()
{
        std::lock_guard<std::mutex> guard(this->data);
        return this->bank;
}

void data::business::set_branch_code(const std::string& branch_num)
{
        if (!branch_num.empty())
        {
                set_flag(6);
                std::lock_guard<std::mutex> guard(this->data);
                this->branch_code = branch_num;
        }
        else
        {
                clear_flag(6);
        }
}

std::string data::business::get_branch_code()
{
        std::lock_guard<std::mutex> guard(this->data);
        return this->branch_code;
}

void data::business::set_account_number(const std::string& account_num)
{
        if (!account_num.empty())
        {
                set_flag(7);
                std::lock_guard<std::mutex> guard(this->data);
                this->account_number = account_num;
        }
        else
        {
                clear_flag(7);
        }
}

std::string data::business::get_account_number()
{
        std::lock_guard<std::mutex> guard(this->data);
        return this->account_number;
}

void data::business::set_client_message(const std::string& message)
{
        if (!message.empty())
        {
                set_flag(8);
                std::lock_guard<std::mutex> guard(this->data);
                this->client_message = message;
        }
        else
        {
                clear_flag(8);
        }
}

std::string data::business::get_client_message()
{
        std::lock_guard<std::mutex> guard(this->data);
        return this->client_message;
}

bool data::business::check_flags()
{
        std::lock_guard<std::mutex> guard(this->data);
        return ((this->flags & 0xff) == 0xff) ? true : false;
}

void data::business::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->data);
        this->flags |= static_cast<uint8_t>(1 << bit);
}

void data::business::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->data);
        this->flags |= static_cast<uint8_t>(0 << bit);
}
