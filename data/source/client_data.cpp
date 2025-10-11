/********************************************************
 * Contents: Client data implementation
 * Author: Dawid J. Blom
 * Date: November 26, 2024
 *
 * NOTE:
 *******************************************************/
#include <client_data.h>


namespace upper_bound {
        constexpr std::uint8_t string_length{50};
        constexpr std::uint8_t statement_format_length{3};
}

data::client::client(const client& _copy)
        : business_name{_copy.business_name}, business_address{_copy.business_address},
          business_area_code{_copy.business_area_code}, business_town_name{_copy.business_town_name},
          cellphone_number{_copy.cellphone_number}, emails{_copy.emails}, vat_number{_copy.vat_number},
          statement_schedule{_copy.statement_schedule}, flags{_copy.flags}, client_data{}, mask{_copy.mask}
{
}

data::client::client(client&& _move)
        : business_name{std::move(_move.business_name)}, business_address{std::move(_move.business_address)},
          business_area_code{std::move(_move.business_area_code)}, business_town_name{std::move(_move.business_town_name)},
          cellphone_number{std::move(_move.cellphone_number)}, emails{std::move(_move.emails)}, vat_number{std::move(_move.vat_number)},
          statement_schedule{std::move(_move.statement_schedule)}, flags{std::move(_move.flags)}, client_data{}, mask{std::move(_move.mask)}
{
        _move.business_name.clear();
        _move.business_address.clear();
        _move.business_area_code.clear();
        _move.business_town_name.clear();
        _move.cellphone_number.clear();
        _move.emails.clear();
        _move.vat_number.clear();
        _move.statement_schedule.clear();
        _move.flags = 0;
        _move.mask = this->mask;
}

data::client& data::client::operator= (const client& _copy)
{
        client temp{_copy};
        std::swap(temp, *this);

        return *this;
}

data::client& data::client::operator= (client&& _move)
{
        std::swap(business_name, _move.business_name);
        std::swap(business_address, _move.business_address);
        std::swap(business_area_code, _move.business_area_code);
        std::swap(business_town_name, _move.business_town_name);
        std::swap(cellphone_number, _move.cellphone_number);
        std::swap(emails, _move.emails);
        std::swap(vat_number, _move.vat_number);
        std::swap(statement_schedule, _move.statement_schedule);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

bool data::client::is_valid() const
{
        return this->check_flags();
}

void data::client::set_business_name(const std::string& _name)
{
        if (!_name.empty() && (_name.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::NAME);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->business_name = _name;
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

void data::client::set_business_address(const std::string& _address)
{
        if (!_address.empty() && (_address.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::ADDRESS);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->business_address = _address;
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

void data::client::set_business_area_code(const std::string& _code)
{
        if (!_code.empty() && (_code.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::AREA_CODE);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->business_area_code = _code;
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

void data::client::set_business_town_name(const std::string& _town_name)
{
        if (!_town_name.empty() && (_town_name.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::TOWN);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->business_town_name = _town_name;
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

void data::client::set_cellphone_number(const std::string& _number)
{
        if (!_number.empty() && (_number.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::CELLPHONE);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->cellphone_number = _number;
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

void data::client::set_email(const std::string& _emails)
{
        utility::word_slicer slicer{};
        std::vector<std::string> sliced_emails{slicer.slice(_emails)};
        if (email_address_good(sliced_emails) == true)
        {
                set_flag(FLAG::EMAIL);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->emails = _emails;
        }
        else
        {
                clear_flag(FLAG::EMAIL);
        }
}

std::string data::client::get_email() const
{
        return this->emails;
}

void data::client::set_vat_number(const std::string& _vat_number)
{
        if (!_vat_number.empty() && (_vat_number.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::VAT_NUMBER);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->vat_number = _vat_number;
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

void data::client::set_statement_schedule(const std::string& _statement_schedule)
{
        std::regex sched_regex(R"(^[1-4],[1-7]$)");
        bool format_correct{std::regex_search(_statement_schedule, sched_regex)};
        if (!_statement_schedule.empty() && format_correct)
        {
                set_flag(FLAG::STATMENT_SCHEDULE);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->statement_schedule = _statement_schedule;
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

bool data::client::email_address_good(const std::vector<std::string>& _emails)
{
        std::regex email_regex(R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");
        for (const auto& email : _emails)
        {
                bool email_format_correct{std::regex_search(email, email_regex)};
                if (!email_format_correct || !(email.length() <= upper_bound::string_length))
                {
                        return false;
                }
        }

        return true;
}

bool data::client::check_flags() const
{
        return (((this->flags & this->mask) == this->mask) && ((this->flags & ~this->mask) == 0));
}

void data::client::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->client_data);
        this->flags |= static_cast<mask_type>(BIT::SET << bit);
}

void data::client::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->client_data);
        this->flags &= ~static_cast<mask_type> (BIT::CLEAR << bit);
}
