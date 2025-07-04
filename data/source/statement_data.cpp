/********************************************************
 * Contents: Statement column definition
 * Author: Dawid J. Blom
 * Date: April 4, 2025
 *
 * NOTE:
 *******************************************************/
#include <statement_data.h>


namespace limits {
        constexpr std::uint8_t max_invoice_length{8};
        constexpr std::uint8_t max_paid_status_length{4};
        constexpr std::uint8_t max_length{20};
        constexpr std::uint8_t max_price_length{14};
}

data::statement::statement(const statement& _copy)
        : invoice_number{_copy.invoice_number}, date{_copy.date},
          order_number{_copy.order_number}, paid_status{_copy.paid_status},
          price{_copy.price}, flags{_copy.flags}, data_mutex{}, mask{_copy.mask}
{
}

data::statement::statement(statement&& _move)
        : invoice_number{std::move(_move.invoice_number)}, date{std::move(_move.date)},
          order_number{std::move(_move.order_number)}, paid_status{std::move(_move.paid_status)},
          price{std::move(_move.price)}, flags{std::move(_move.flags)}, data_mutex{},
          mask{std::move(_move.mask)}
{
        _move.invoice_number.clear();
        _move.date.clear();
        _move.order_number.clear();
        _move.paid_status.clear();
        _move.price.clear();
        _move.flags = 0x0;
        _move.mask = this->mask;
}

data::statement& data::statement::operator= (const statement& _copy)
{
        statement temp{_copy};
        std::swap(*this, temp);

        return *this;
}

data::statement& data::statement::operator= (statement&& _move)
{
        std::swap(invoice_number, _move.invoice_number);
        std::swap(date, _move.date);
        std::swap(order_number, _move.order_number);
        std::swap(paid_status, _move.paid_status);
        std::swap(price, _move.price);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

bool data::statement::is_valid() const
{
	return check_flags();
}

void data::statement::set_invoice_number(const std::string& _invoice_number)
{
        if (!_invoice_number.empty() && _invoice_number.length() <= limits::max_invoice_length)
        {
                set_flag(FLAG::INVOICE_NUMBER);
                std::lock_guard<std::mutex> guard(this->data_mutex);
                this->invoice_number = std::move(_invoice_number);
        }
        else
        {
                clear_flag(FLAG::INVOICE_NUMBER);
        }
}

std::string data::statement::get_invoice_number() const
{
        return this->invoice_number;
}

void data::statement::set_date(const std::string& _date)
{
        if (!_date.empty() && _date.length() <= limits::max_length)
        {
                set_flag(FLAG::DATE);
                std::lock_guard<std::mutex> guard(this->data_mutex);
                this->date = std::move(_date);
        }
        else
        {
                clear_flag(FLAG::DATE);
        }
}

std::string data::statement::get_date() const
{
        return this->date;
}

void data::statement::set_order_number(const std::string& _order_number)
{
        if (!_order_number.empty() && _order_number.length() <= limits::max_length)
        {
                set_flag(FLAG::ORDER_NUMBER);
                std::lock_guard<std::mutex> guard(this->data_mutex);
                this->order_number = std::move(_order_number);
        }
        else
        {
                clear_flag(FLAG::ORDER_NUMBER);
        }
}

std::string data::statement::get_order_number() const
{
        return this->order_number;
}

void data::statement::set_paid_status(const std::string& _paid_status)
{
        if (!_paid_status.empty() && _paid_status.length() <= limits::max_paid_status_length)
        {
                set_flag(FLAG::PAID_STATUS);
                std::lock_guard<std::mutex> guard(this->data_mutex);
                this->paid_status = std::move(_paid_status);
        }
        else
        {
                clear_flag(FLAG::PAID_STATUS);
        }
}

std::string data::statement::get_paid_status() const
{
        return this->paid_status;
}

void data::statement::set_price(const std::string& _price)
{
        if (!_price.empty() && _price.length() <= limits::max_price_length)
        {
                set_flag(FLAG::PRICE);
                std::lock_guard<std::mutex> guard(this->data_mutex);
                this->price = std::move(_price);
        }
        else
        {
                clear_flag(FLAG::PRICE);
        }
}

std::string data::statement::get_price() const
{
        return this->price;
}

bool data::statement::check_flags() const
{
        return (((this->flags & this->mask) == this->mask) && ((this->flags & ~this->mask) == 0));
}

void data::statement::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        this->flags |= static_cast<mask_type> (BIT::SET << bit);
}

void data::statement::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        this->flags &= ~static_cast<mask_type> (BIT::CLEAR << bit);
}
