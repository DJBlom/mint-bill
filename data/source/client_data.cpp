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

data::client::~client() {}

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
