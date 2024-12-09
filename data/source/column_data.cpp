/********************************************************
 * Contents: Column data implementation
 * Author: Dawid J. Blom
 * Date: December 9, 2024
 *
 * NOTE:
 *******************************************************/
#include <column_data.h>
#include <sstream>
#include <iomanip>

namespace limit {
        constexpr std::uint8_t MAX_QUANTITY{9};
        constexpr std::uint16_t MAX_DESCRIPTION{500};
        constexpr std::uint8_t MAX_AMOUNT{20};
}

data::column::column() {}

data::column::column(const column& new_data)
        : quantity{new_data.quantity}, description{new_data.description},
          amount{new_data.amount}, flags{new_data.flags}, column_data{},
          mask{new_data.mask} {}

data::column::column(column&& new_data)
        : quantity{new_data.quantity}, description{new_data.description},
          amount{new_data.amount}, flags{new_data.flags}, column_data{},
          mask{new_data.mask}
{
        new_data.set_quantity(0);
        new_data.set_description("");
        new_data.set_amount(0.0);
        new_data.flags = 0;
        new_data.mask = 0x7;
}

data::column& data::column::operator= (const column& new_data)
{
        column temp{new_data};
        std::swap(temp, *this);

        return *this;
}

data::column& data::column::operator= (column&& new_data)
{
        std::swap(quantity, new_data.quantity);
        std::swap(description, new_data.description);
        std::swap(amount, new_data.amount);
        std::swap(flags, new_data.flags);
        std::swap(mask, new_data.mask);

        return *this;
}

data::column::~column() {}

bool data::column::is_valid() const
{
        bool is_valid{false};
        if (this->check_flags() == true)
        {
                is_valid = true;
        }

        return is_valid;
}

void data::column::set_quantity(const unsigned int& quan)
{
        std::string tmp{std::to_string(quan)};
        if (tmp.length() <= limit::MAX_QUANTITY)
        {
                this->set_flag(FLAG::QUANTITY);
                std::lock_guard<std::mutex> guard(this->column_data);
                this->quantity = quan;
        }
        else
        {
                this->clear_flag(FLAG::QUANTITY);
        }
}

unsigned int data::column::get_quantity()
{
        return this->quantity;
}

void data::column::set_description(const std::string& desc)
{
        if (!desc.empty() && (desc.length() <= limit::MAX_DESCRIPTION))
        {
                this->set_flag(FLAG::DESCRIPTION);
                std::lock_guard<std::mutex> guard(this->column_data);
                this->description = desc;
        }
        else
        {
                this->clear_flag(FLAG::DESCRIPTION);
        }
}

std::string data::column::get_description()
{
        return this->description;
}

void data::column::set_amount(const double& am)
{
        std::ostringstream amss{std::to_string(am)};
        amss << std::fixed << std::setprecision(2) << am;
        std::string tmp{amss.str()};
        if (tmp.length() <= limit::MAX_AMOUNT)
        {
                this->set_flag(FLAG::AMOUNT);
                std::lock_guard<std::mutex> guard(this->column_data);
                this->amount = am;
        }
        else
        {
                this->clear_flag(FLAG::AMOUNT);
        }
}

double data::column::get_amount()
{
        return this->amount;
}

bool data::column::check_flags() const
{
        return ((this->flags & this->mask) == 0x7) ? true : false;
}

void data::column::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->column_data);
        this->flags |= static_cast<std::uint8_t>(BIT::SET << bit);
}

void data::column::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->column_data);
        this->flags |= static_cast<std::uint8_t>(BIT::UNSET << bit);
}
