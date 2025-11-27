#include <statement_data.h>


namespace upper_bound {
        constexpr std::uint8_t name_length{50};
        constexpr std::uint8_t string_length{20};
}

data::statement::statement(){}

data::statement::~statement() {}

data::statement::statement(const statement& _copy)
        : business_name{_copy.business_name}, period_start{_copy.period_start},
          period_end{_copy.period_end}, schedule{_copy.schedule}, statement_date{_copy.statement_date},
	  flags{_copy.flags}, statement_data{}, mask{_copy.mask}
{
}

data::statement::statement(statement&& _move)
        : business_name{std::move(_move.business_name)}, period_start{std::move(_move.period_start)},
          period_end{std::move(_move.period_end)}, schedule{std::move(_move.schedule)}, statement_date{std::move(_move.statement_date)},
	  flags{std::move(_move.flags)}, statement_data{}, mask{std::move(_move.mask)}
{
	_move.business_name.clear();
	_move.period_start.clear();
	_move.period_end.clear();
	_move.schedule.clear();
	_move.statement_date.clear();
	_move.flags = 0x0;
	_move.mask = this->mask;
}

data::statement& data::statement::operator= (const statement& _copy)
{
        statement temp{_copy};
        std::swap(temp, *this);

        return *this;
}

data::statement& data::statement::operator= (statement&& _move)
{
        std::swap(business_name, _move.business_name);
        std::swap(period_start, _move.period_start);
        std::swap(period_end, _move.period_end);
        std::swap(schedule, _move.schedule);
        std::swap(statement_date, _move.statement_date);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

bool data::statement::is_valid() const
{
        return this->check_flags();
}

void data::statement::set_business_name(const std::string& _name)
{
        if (!_name.empty() && (_name.length() <= upper_bound::name_length))
        {
                set_flag(FLAG::NAME);
                std::lock_guard<std::mutex> guard(this->statement_data);
                this->business_name = std::move(_name);
        }
        else
        {
                clear_flag(FLAG::NAME);
        }
}

std::string data::statement::get_business_name() const
{
        return this->business_name;
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

void data::statement::set_schedule(const std::string& _schedule)
{
        std::regex sched_regex(R"(^[1-4],[1-7]$)");
        bool format_correct{std::regex_search(_schedule, sched_regex)};
        if (!_schedule.empty() && format_correct)
        {
                set_flag(FLAG::SCHEDULE);
                std::lock_guard<std::mutex> guard(this->statement_data);
                this->schedule = std::move(_schedule);
        }
        else
        {
                clear_flag(FLAG::SCHEDULE);
        }
}

std::string data::statement::get_schedule() const
{
        return this->schedule;
}

void data::statement::set_statement_date(const std::string& _date)
{
        if (!_date.empty() && (_date.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::DATE);
                std::lock_guard<std::mutex> guard(this->statement_data);
                this->statement_date = std::move(_date);
        }
        else
        {
                clear_flag(FLAG::DATE);
        }
}

std::string data::statement::get_statement_date() const
{
        return this->statement_date;
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
