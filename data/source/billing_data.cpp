/*****************************************************************************
 * @file billing_data.cpp
 *
 * @brief
 *   Implements the billing data model defined in billing_data.h, including
 *   construction, assignment, validation, and controlled mutation of billing
 *   attributes.
 *
 * @details
 *   This implementation defines the behavior of data::billing, including copy
 *   and move semantics, as well as setter and getter functions for each of the
 *   core billing fields: identifier, name, date, and paid status. Each setter
 *   applies simple string length checks before updating internal state and
 *   recording the result in the flag mask.
 *
 *   The check_flags() helper consolidates the flag state to determine whether
 *   all required fields are set, allowing is_valid() to provide a clear
 *   indication of completeness. A mutex is used to guard writes to internal
 *   members so that billing records can be safely modified in multithreaded
 *   environments.
 *
 * @notes
 *   Field length limits are provided via the local upper_bound namespace,
 *   allowing the constraints to be tuned without changing the core class
 *   interface. Callers should rely on is_valid() to test completeness rather
 *   than assuming setter calls always yield a valid object.
 *****************************************************************************/
#include <billing_data.h>

namespace upper_bound {
        constexpr std::uint8_t name_length{50};
        constexpr std::uint8_t string_length{20};
}

data::billing::billing(){}

data::billing::~billing() {}

data::billing::billing(const billing& _copy)
        : id{_copy.id}, name{_copy.name}, date{_copy.date}, paid_status{_copy.paid_status},
	  flags{_copy.flags}, billing_data{}, mask{_copy.mask}
{
}

data::billing::billing(billing&& _move)
        : id{std::move(_move.id)}, name{std::move(_move.name)}, date{std::move(_move.date)},
	  paid_status{std::move(_move.paid_status)}, flags{std::move(_move.flags)},
	  billing_data{}, mask{std::move(_move.mask)}
{
	_move.id.clear();
	_move.name.clear();
	_move.date.clear();
	_move.paid_status.clear();
	_move.flags = 0x0;
	_move.mask = this->mask;
}

data::billing& data::billing::operator= (const billing& _copy)
{
        billing temp{_copy};
        std::swap(temp, *this);

        return *this;
}

data::billing& data::billing::operator= (billing&& _move)
{
        std::swap(id, _move.id);
        std::swap(name, _move.name);
        std::swap(date, _move.date);
        std::swap(paid_status, _move.paid_status);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

bool data::billing::is_valid() const
{
        return this->check_flags();
}

void data::billing::set_id(const std::string& _id)
{
        if (!_id.empty() && (_id.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::ID);
                std::lock_guard<std::mutex> guard(this->billing_data);
                this->id = std::move(_id);
        }
        else
        {
                clear_flag(FLAG::ID);
        }
}

std::string data::billing::get_id() const
{
        return this->id;
}

void data::billing::set_name(const std::string& _name)
{
        if (!_name.empty() && (_name.length() <= upper_bound::name_length))
        {
                set_flag(FLAG::NAME);
                std::lock_guard<std::mutex> guard(this->billing_data);
                this->name = std::move(_name);
        }
        else
        {
                clear_flag(FLAG::NAME);
        }
}

std::string data::billing::get_name() const
{
        return this->name;
}

void data::billing::set_date(const std::string& _date)
{
        if (!_date.empty() && (_date.length() <= upper_bound::name_length))
        {
                set_flag(FLAG::DATE);
                std::lock_guard<std::mutex> guard(this->billing_data);
                this->date = std::move(_date);
        }
        else
        {
                clear_flag(FLAG::DATE);
        }
}

std::string data::billing::get_date() const
{
        return this->date;
}

void data::billing::set_paid_status(const std::string& _status)
{
        if (!_status.empty() && (_status.length() <= upper_bound::name_length))
        {
                set_flag(FLAG::PAID_STATUS);
                std::lock_guard<std::mutex> guard(this->billing_data);
                this->paid_status = std::move(_status);
        }
        else
        {
                clear_flag(FLAG::PAID_STATUS);
        }
}

std::string data::billing::get_paid_status() const
{
        return this->paid_status;
}

bool data::billing::check_flags() const
{
        return (((this->flags & this->mask) == this->mask) && ((this->flags & ~this->mask) == 0));
}

void data::billing::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->billing_data);
        this->flags |= static_cast<mask_type>(BIT::SET << bit);
}

void data::billing::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->billing_data);
        this->flags &= ~static_cast<mask_type> (BIT::CLEAR << bit);
}
