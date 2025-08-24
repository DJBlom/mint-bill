/********************************************************
 * Contents: Column data implementation
 * Author: Dawid J. Blom
 * Date: December 9, 2024
 *
 * NOTE:
 *******************************************************/
#include <column_data.h>
#include <iostream>


namespace limit {
        constexpr std::uint8_t MAX_QUANTITY{9};
        constexpr std::uint16_t MAX_DESCRIPTION{500};
        constexpr std::uint8_t MAX_AMOUNT{15};
}

data::column::column() {}

data::column::column(const column& _copy)
        : quantity{_copy.quantity}, description{_copy.description},
          amount{_copy.amount}, flags{_copy.flags}, column_data{},
          mask{_copy.mask} {}

data::column::column(column&& _move)
        : quantity{std::move(_move.quantity)}, description{std::move(_move.description)},
          amount{std::move(_move.amount)}, flags{std::move(_move.flags)}, column_data{},
          mask{std::move(_move.mask)}
{
        _move.quantity = 0;
        _move.description.clear();
        _move.amount = 0.0;
        _move.flags = 0;
        _move.mask = this->mask;
}

data::column& data::column::operator= (const column& _copy)
{
        column temp{_copy};
        std::swap(temp, *this);

        return *this;
}

data::column& data::column::operator= (column&& _move)
{
        std::swap(quantity, _move.quantity);
        std::swap(description, _move.description);
        std::swap(amount, _move.amount);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

data::column::~column() {}

bool data::column::is_valid() const
{
        return this->check_flags();
}

void data::column::set_quantity(const unsigned int& _quantity)
{
        std::string tmp{std::to_string(_quantity)};
        if (tmp.length() <= limit::MAX_QUANTITY)
        {
                this->set_flag(FLAG::QUANTITY);
                std::lock_guard<std::mutex> guard(this->column_data);
                this->quantity = std::move(_quantity);
        }
        else
        {
                this->clear_flag(FLAG::QUANTITY);
        }
}

unsigned int data::column::get_quantity() const
{
        return this->quantity;
}

void data::column::set_description(const std::string& _description)
{
        if (!_description.empty() && (_description.length() <= limit::MAX_DESCRIPTION))
        {
                this->set_flag(FLAG::DESCRIPTION);
                std::lock_guard<std::mutex> guard(this->column_data);
                this->description = std::move(_description);
        }
        else
        {
                this->clear_flag(FLAG::DESCRIPTION);
        }
}

std::string data::column::get_description() const
{
        return this->description;
}

void data::column::set_amount(const double& _amount)
{
        std::ostringstream amss{};
        amss.imbue(std::locale::classic());
        amss << std::fixed << std::setprecision(2) << _amount;
        std::string tmp{amss.str()};
        if (tmp.length() <= limit::MAX_AMOUNT)
        {
                this->set_flag(FLAG::AMOUNT);
                std::lock_guard<std::mutex> guard(this->column_data);
                this->amount = std::move(_amount);
        }
        else
        {
                this->clear_flag(FLAG::AMOUNT);
        }
}

double data::column::get_amount() const
{
        return this->amount;
}

bool data::column::check_flags() const
{
        return (((this->flags & this->mask) == this->mask) && ((this->flags & ~this->mask) == 0));
}

void data::column::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->column_data);
        this->flags |= static_cast<mask_type>(BIT::SET << bit);
}

void data::column::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->column_data);
        this->flags &= ~static_cast<mask_type> (BIT::CLEAR << bit);
}
