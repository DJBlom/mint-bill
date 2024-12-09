/********************************************************
 * Contents: Invoice data implementation
 * Author: Dawid J. Blom
 * Date: December 4, 2024
 *
 * NOTE:
 *******************************************************/
#include <iostream>
#include <invoice_data.h>


namespace upper_bound {
        constexpr std::uint8_t string_length{50};
}

data::invoice::invoice()
{

}

data::invoice::~invoice()
{

}

data::invoice::invoice(const invoice& new_data)
        : business_name{new_data.business_name}, invoice_number{new_data.invoice_number},
          job_card_number{new_data.job_card_number}, order_number{new_data.order_number},
          description_column{new_data.description_column}, material_column{new_data.material_column},
          flags{new_data.flags}, invoice_data{}, mask{new_data.mask}
{
}

data::invoice::invoice(invoice&& new_data)
        : business_name{new_data.business_name}, invoice_number{new_data.invoice_number},
          job_card_number{new_data.job_card_number}, order_number{new_data.order_number},
          description_column{new_data.description_column}, material_column{new_data.material_column},
          flags{new_data.flags}, invoice_data{}, mask{new_data.mask}
{
        new_data.business_name.clear();
        new_data.invoice_number = 0;
        new_data.job_card_number.clear();
        new_data.order_number.clear();
        new_data.description_column.clear();
        new_data.material_column.clear();
        new_data.flags = 0;
        new_data.mask = 0x7F;
}

data::invoice& data::invoice::operator= (const invoice& new_data)
{
        invoice temp{new_data};
        std::swap(temp, *this);

        return *this;
}

data::invoice& data::invoice::operator= (invoice&& new_data)
{
        std::swap(business_name, new_data.business_name);
        std::swap(invoice_number, new_data.invoice_number);
        std::swap(job_card_number, new_data.job_card_number);
        std::swap(order_number, new_data.order_number);
        std::swap(description_column, new_data.description_column);
        std::swap(material_column, new_data.material_column);
        std::swap(flags, new_data.flags);
        std::swap(mask, new_data.mask);

        return *this;
}

bool data::invoice::is_valid() const
{
        bool is_valid{false};
        if (this->check_flags() == true)
        {
                is_valid = true;
        }

        return is_valid;
}

void data::invoice::set_business_name(const std::string& name)
{
        if (!name.empty() && (name.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::NAME);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->business_name = name;
        }
        else
        {
                clear_flag(FLAG::NAME);
        }
}

std::string data::invoice::get_business_name() const
{
        return this->business_name;
}

void data::invoice::set_invoice_number(const unsigned int& number)
{
        if (number > 0)
        {
                set_flag(FLAG::NUMBER);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->invoice_number = number;
        }
        else
        {
                clear_flag(FLAG::NUMBER);
        }
}

unsigned int data::invoice::get_invoice_number() const
{
        return this->invoice_number;
}

void data::invoice::set_invoice_date(const std::string& date)
{
        if (!date.empty() && (date.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::DATE);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->invoice_date = date;
        }
        else
        {
                clear_flag(FLAG::DATE);
        }
}

std::string data::invoice::get_invoice_date() const
{
        return this->invoice_date;
}

void data::invoice::set_job_card_number(const std::string& card_number)
{
        if (!card_number.empty() && (card_number.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::JOB_CARD);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->job_card_number = card_number;
        }
        else
        {
                clear_flag(FLAG::JOB_CARD);
        }
}

std::string data::invoice::get_job_card_number() const
{
        return this->job_card_number;
}

void data::invoice::set_order_number(const std::string& number)
{
        if (!number.empty() && (number.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::ORDER_NUMBER);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->order_number = number;
        }
        else
        {
                clear_flag(FLAG::ORDER_NUMBER);
        }
}

std::string data::invoice::get_order_number() const
{
        return this->order_number;
}

void data::invoice::set_description_column(const std::vector<data::column>& col)
{
        if (!col.empty())
        {
                set_flag(FLAG::DESCRIPTION_COL);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->description_column = std::move(col);
        }
        else
        {
                clear_flag(FLAG::DESCRIPTION_COL);
        }
}

std::vector<data::column> data::invoice::get_description_column()
{
        return std::move(this->description_column);
}

void data::invoice::set_material_column(const std::vector<data::column>& col)
{
        if (!col.empty())
        {
                set_flag(FLAG::MATERIAL_COL);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->material_column = std::move(col);
        }
        else
        {
                clear_flag(FLAG::MATERIAL_COL);
        }
}

std::vector<data::column> data::invoice::get_material_column()
{
        return std::move(this->material_column);
}

bool data::invoice::check_flags() const
{
        return ((this->flags & this->mask) == 0x7F) ? true : false;
}

void data::invoice::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->invoice_data);
        this->flags |= static_cast<std::uint8_t>(BIT::SET << bit);
}

void data::invoice::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->invoice_data);
        this->flags |= static_cast<std::uint8_t>(BIT::UNSET << bit);
}
