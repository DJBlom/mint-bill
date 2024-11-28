/********************************************************
 * Contents: Client data implementation
 * Author: Dawid J. Blom
 * Date: November 26, 2024
 *
 * NOTE:
 *******************************************************/
#include <client_data.h>
#include <regex>


namespace upper_bound {
        constexpr std::uint8_t string_length{50};
        constexpr std::uint8_t statement_format_length{3};
}

data::client::client(const client& new_data)
        : business_name{new_data.business_name}, business_address{new_data.business_address},
          business_area_code{new_data.business_area_code}, business_town_name{new_data.business_town_name},
          cellphone_number{new_data.cellphone_number}, email{new_data.email}, vat_number{new_data.vat_number},
          statement_schedule{new_data.statement_schedule}, flags{new_data.flags}, client_data{}, mask{new_data.mask}
{
}

data::client::client(client&& new_data)
        : business_name{new_data.business_name}, business_address{new_data.business_address},
          business_area_code{new_data.business_area_code}, business_town_name{new_data.business_town_name},
          cellphone_number{new_data.cellphone_number}, email{new_data.email}, vat_number{new_data.vat_number},
          statement_schedule{new_data.statement_schedule}, flags{new_data.flags}, client_data{}, mask{new_data.mask}
{
        new_data.business_name.clear();
        new_data.business_address.clear();
        new_data.business_area_code.clear();
        new_data.business_town_name.clear();
        new_data.cellphone_number.clear();
        new_data.email.clear();
        new_data.vat_number.clear();
        new_data.statement_schedule.clear();
        new_data.flags = 0;
        new_data.mask = 0x0;
}

data::client& data::client::operator= (const client& new_data)
{
        client temp{new_data};
        std::swap(temp, *this);

        return *this;
}

data::client& data::client::operator= (client&& new_data)
{
        std::swap(business_name, new_data.business_name);
        std::swap(business_address, new_data.business_address);
        std::swap(business_area_code, new_data.business_area_code);
        std::swap(business_town_name, new_data.business_town_name);
        std::swap(cellphone_number, new_data.cellphone_number);
        std::swap(email, new_data.email);
        std::swap(vat_number, new_data.vat_number);
        std::swap(statement_schedule, new_data.statement_schedule);
        std::swap(flags, new_data.flags);
        std::swap(mask, new_data.mask);

        return *this;
}

bool data::client::is_valid() const
{
        bool is_valid{false};
        if (this->check_flags() == true)
        {
                is_valid = true;
        }

        return is_valid;
}

void data::client::set_business_name(const std::string& name)
{
        if (!name.empty() && (name.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::NAME);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->business_name = name;
        }
        else
        {
                clear_flag(FLAG::NAME);
        }
}

std::string data::client::get_business_name() const
{
        return this->business_name;
}

void data::client::set_business_address(const std::string& address)
{
        if (!address.empty() && (address.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::ADDRESS);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->business_address = address;
        }
        else
        {
                clear_flag(FLAG::ADDRESS);
        }
}

std::string data::client::get_business_address() const
{
        return this->business_address;
}

void data::client::set_business_area_code(const std::string& code)
{
        if (!code.empty() && (code.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::AREA_CODE);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->business_area_code = code;
        }
        else
        {
                clear_flag(FLAG::AREA_CODE);
        }
}

std::string data::client::get_business_area_code() const
{
        return this->business_area_code;
}

void data::client::set_business_town_name(const std::string& town_name)
{
        if (!town_name.empty() && (town_name.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::TOWN);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->business_town_name = town_name;
        }
        else
        {
                clear_flag(FLAG::TOWN);
        }
}

std::string data::client::get_business_town_name() const
{
        return this->business_town_name;
}

void data::client::set_cellphone_number(const std::string& number)
{
        if (!number.empty() && (number.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::CELLPHONE);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->cellphone_number = number;
        }
        else
        {
                clear_flag(FLAG::CELLPHONE);
        }
}

std::string data::client::get_cellphone_number() const
{
        return this->cellphone_number;
}

void data::client::set_email(const std::string& email_address)
{
        std::regex email_regex(R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");
        bool email_format_correct{std::regex_search(email_address, email_regex)};
        if (email_format_correct && (email_address.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::EMAIL);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->email = email_address;
        }
        else
        {
                clear_flag(FLAG::EMAIL);
        }
}

std::string data::client::get_email() const
{
        return this->email;
}

void data::client::set_vat_number(const std::string& vat)
{
        if (!vat.empty() && (vat.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::VAT_NUMBER);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->vat_number = vat;
        }
        else
        {
                clear_flag(FLAG::VAT_NUMBER);
        }
}

std::string data::client::get_vat_number() const
{
        return this->vat_number;
}

void data::client::set_statement_schedule(const std::string& schedule)
{
        if (!schedule.empty() && schedule.contains(",") && (schedule.length() == upper_bound::statement_format_length))
        {
                set_flag(FLAG::STATMENT_SCHEDULE);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->statement_schedule = schedule;
        }
        else
        {
                clear_flag(FLAG::STATMENT_SCHEDULE);
        }
}

std::string data::client::get_statement_schedule() const
{
        return this->statement_schedule;
}

bool data::client::check_flags() const
{
        return ((this->flags & this->mask) == 0xFF) ? true : false;
}

void data::client::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->client_data);
        this->flags |= static_cast<std::uint8_t>(1 << bit);
}

void data::client::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->client_data);
        this->flags |= static_cast<std::uint8_t>(0 << bit);
}
