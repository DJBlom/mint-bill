#include <statement_column.h>
#include <iostream>


namespace limits {
        constexpr std::uint8_t max_invoice_length{8};
}


bool data::statement_column::is_valid()
{
        if ((this->flags & this->mask) == this->mask)
        {
                return true;
        }
        else
        {
                return false;
        }
}

void data::statement_column::set_invoice_number(const std::string& _invoice_number)
{
        if (!_invoice_number.empty() && _invoice_number.length() <= limits::max_invoice_length)
        {
                set_flags(FLAG::INVOICE_NUMBER);
                this->invoice_number = std::move(_invoice_number);
        }
        else
        {
                clear_flags(FLAG::INVOICE_NUMBER);
        }
}

std::string data::statement_column::get_invoice_number()
{
        return this->invoice_number;
}

void data::statement_column::set_flags(const int& bit)
{
        this->flags |= static_cast<std::uint8_t> (BIT::SET << bit);
}

void data::statement_column::clear_flags(const int& bit)
{
        this->flags |= static_cast<std::uint8_t> (BIT::CLEAR << bit);
}
