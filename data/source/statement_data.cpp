/*****************************************************************************
 * @file statement_data.cpp
 *
 * @brief
 *   Implements the statement data model defined in statement_data.h, including
 *   construction, assignment, validation, and controlled updates to statement
 *   period fields.
 *
 * @details
 *   This implementation defines the behavior of data::statement, providing
 *   copy and move semantics and setter/getter methods for the period start and
 *   end strings. Setter functions enforce simple non-empty and length
 *   constraints before committing updates and recording the result in the
 *   internal flag mask.
 *
 *   The check_flags() helper consolidates statement-level flag state, allowing
 *   is_valid() to combine period completeness with data::billing::is_valid()
 *   for an overall correctness check. A mutex guards modifications to the
 *   period fields, supporting thread-safe usage in concurrent code.
 *
 * @notes
 *   Length limits for period strings are defined in the local upper_bound
 *   namespace, making it easy to tune constraints without modifying the public
 *   interface. Callers should rely on is_valid() to confirm that a statement
 *   instance is properly configured before using it in downstream processing,
 *   such as PDF generation or reporting.
 *****************************************************************************/
#include <statement_data.h>


namespace upper_bound {
        constexpr std::uint8_t name_length{50};
        constexpr std::uint8_t string_length{20};
}

data::statement::statement(){}

data::statement::~statement() {}

data::statement::statement(const statement& _copy)
        : data::billing{_copy}, period_start{_copy.period_start},
          period_end{_copy.period_end}, flags{_copy.flags},
	  statement_data{}, mask{_copy.mask}
{
}

data::statement::statement(statement&& _move)
        : data::billing{std::move(_move)}, period_start{std::move(_move.period_start)},
          period_end{std::move(_move.period_end)}, flags{std::move(_move.flags)},
	  statement_data{}, mask{std::move(_move.mask)}
{
	_move.period_start.clear();
	_move.period_end.clear();
	_move.flags = 0x0;
	_move.mask = this->mask;
}

data::statement& data::statement::operator= (const statement& _copy)
{
        statement temp{_copy};
        std::swap(temp, *this);
	data::billing::operator=(_copy);

        return *this;
}

data::statement& data::statement::operator= (statement&& _move)
{
	data::billing::operator=(_move);
        std::swap(period_start, _move.period_start);
        std::swap(period_end, _move.period_end);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

bool data::statement::is_valid() const
{
        return this->check_flags() && data::billing::is_valid();
}

void data::statement::set_period_start(const std::string& _period_start)
{
        if (!_period_start.empty() && (_period_start.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::PERIOD_START);
                std::lock_guard<std::mutex> guard(this->statement_data);
                this->period_start = std::move(_period_start);
        }
        else
        {
                clear_flag(FLAG::PERIOD_START);
        }
}

std::string data::statement::get_period_start() const
{
        return this->period_start;
}

void data::statement::set_period_end(const std::string& _period_end)
{
        if (!_period_end.empty() && (_period_end.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::PERIOD_END);
                std::lock_guard<std::mutex> guard(this->statement_data);
                this->period_end = std::move(_period_end);
        }
        else
        {
                clear_flag(FLAG::PERIOD_END);
        }
}

std::string data::statement::get_period_end() const
{
        return this->period_end;
}

bool data::statement::check_flags() const
{
        return (((this->flags & this->mask) == this->mask) && ((this->flags & ~this->mask) == 0));
}

void data::statement::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->statement_data);
        this->flags |= static_cast<mask_type>(BIT::SET << bit);
}

void data::statement::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->statement_data);
        this->flags &= ~static_cast<mask_type> (BIT::CLEAR << bit);
}
