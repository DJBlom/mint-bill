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
	: data::business{_copy}, vat_number{_copy.vat_number},
	  statement_schedule{_copy.statement_schedule}, flags{_copy.flags},
	  client_data{}, mask{_copy.mask}
{
}

data::client::client(client&& _move)
	: data::business{std::move(_move)}, vat_number{std::move(_move.vat_number)},
          statement_schedule{std::move(_move.statement_schedule)}, flags{std::move(_move.flags)},
	  client_data{}, mask{std::move(_move.mask)}
{
        _move.vat_number.clear();
        _move.statement_schedule.clear();
        _move.flags = 0;
        _move.mask = this->mask;
}

data::client& data::client::operator= (const client& _copy)
{
        client temp{_copy};
        std::swap(temp, *this);
	data::business::operator=(_copy);

        return *this;
}

data::client& data::client::operator= (client&& _move)
{
	data::business::operator=(_move);
        std::swap(vat_number, _move.vat_number);
        std::swap(statement_schedule, _move.statement_schedule);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

bool data::client::is_valid() const
{
        return this->check_flags() && data::business::is_valid();
}

void data::client::set_name(const std::string& _name)
{
	data::business::set_name(_name);
}

std::string data::client::get_name() const
{
	return data::business::get_name();
}

void data::client::set_address(const std::string& _address)
{
	data::business::set_address(_address);
}

std::string data::client::get_address() const
{
	return data::business::get_address();
}

void data::client::set_area_code(const std::string& _code)
{
	data::business::set_area_code(_code);
}

std::string data::client::get_area_code() const
{
	return data::business::get_area_code();
}

void data::client::set_town(const std::string& _town_name)
{
	data::business::set_town(_town_name);
}

std::string data::client::get_town() const
{
	return data::business::get_town();
}

void data::client::set_cellphone(const std::string& _number)
{
	data::business::set_cellphone(_number);
}

std::string data::client::get_cellphone() const
{
	return data::business::get_cellphone();
}

void data::client::set_email(const std::string& _email_addresses)
{
	data::business::set_email(_email_addresses);
}

std::string data::client::get_email() const
{
	return data::business::get_email();
}

void data::client::set_vat_number(const std::string& _vat_number)
{
        if (!_vat_number.empty() && (_vat_number.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::VAT_NUMBER);
                std::lock_guard<std::mutex> guard(this->client_data);
                this->vat_number = std::move(_vat_number);
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
                this->statement_schedule = std::move(_statement_schedule);
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
