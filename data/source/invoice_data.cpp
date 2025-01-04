/********************************************************
 * Contents: Invoice data implementation
 * Author: Dawid J. Blom
 * Date: December 4, 2024
 *
 * NOTE:
 *******************************************************/
#include <invoice_data.h>


namespace upper_bound {
        constexpr std::uint8_t string_length{20};
        constexpr std::uint8_t invoice_number_length{8};
}

data::invoice::invoice()
{

}

data::invoice::~invoice()
{

}

data::invoice::invoice(const invoice& _copy)
        : business_name{_copy.business_name}, invoice_number{_copy.invoice_number}, invoice_date{_copy.invoice_date},
          job_card_number{_copy.job_card_number}, order_number{_copy.order_number},
          description_total{_copy.description_total}, material_total{_copy.material_total},
          grand_total{_copy.grand_total}, description_column{_copy.description_column},
          material_column{_copy.material_column}, flags{_copy.flags}, invoice_data{},
          mask{_copy.mask}
{
}

data::invoice::invoice(invoice&& _move)
        : business_name{_move.business_name}, invoice_number{_move.invoice_number}, invoice_date{_move.invoice_date},
          job_card_number{_move.job_card_number}, order_number{_move.order_number},
          description_total{_move.description_total}, material_total{_move.material_total},
          grand_total{_move.grand_total}, description_column{_move.description_column},
          material_column{_move.material_column}, flags{_move.flags}, invoice_data{},
          mask{_move.mask}
{
        _move.business_name.clear();
        _move.invoice_number.clear();
        _move.invoice_date.clear();
        _move.job_card_number.clear();
        _move.order_number.clear();
        _move.description_total.clear();
        _move.material_total.clear();
        _move.grand_total.clear();
        _move.description_column.clear();
        _move.material_column.clear();
        _move.flags = 0;
        _move.mask = this->mask;
}

data::invoice& data::invoice::operator= (const invoice& _copy)
{
        invoice temp{_copy};
        std::swap(temp, *this);

        return *this;
}

data::invoice& data::invoice::operator= (invoice&& _move)
{
        std::swap(business_name, _move.business_name);
        std::swap(invoice_number, _move.invoice_number);
        std::swap(invoice_date, _move.invoice_date);
        std::swap(job_card_number, _move.job_card_number);
        std::swap(order_number, _move.order_number);
        std::swap(description_total, _move.description_total);
        std::swap(material_total, _move.material_total);
        std::swap(grand_total, _move.grand_total);
        std::swap(description_column, _move.description_column);
        std::swap(material_column, _move.material_column);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

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

void data::invoice::set_business_name(const std::string& _name)
{
        if (!_name.empty() && (_name.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::NAME);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->business_name = std::move(_name);
        }
        else
        {
                clear_flag(FLAG::NAME);
        }
}

std::string data::invoice::get_business_name() const
{
        return std::move(this->business_name);
}

void data::invoice::set_invoice_number(const std::string& _invoice_number)
{
        if (_invoice_number.length() <= upper_bound::invoice_number_length)
        {
                set_flag(FLAG::NUMBER);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->invoice_number = std::move(_invoice_number);
        }
        else
        {
                clear_flag(FLAG::NUMBER);
        }
}

std::string data::invoice::get_invoice_number() const
{
        return std::move(this->invoice_number);
}

void data::invoice::set_invoice_date(const std::string& _date)
{
        if (!_date.empty() && (_date.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::DATE);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->invoice_date = std::move(_date);
        }
        else
        {
                clear_flag(FLAG::DATE);
        }
}

std::string data::invoice::get_invoice_date() const
{
        return std::move(this->invoice_date);
}

void data::invoice::set_job_card_number(const std::string& _card_number)
{
        if (!_card_number.empty() && (_card_number.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::JOB_CARD);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->job_card_number = std::move(_card_number);
        }
        else
        {
                clear_flag(FLAG::JOB_CARD);
        }
}

std::string data::invoice::get_job_card_number() const
{
        return std::move(this->job_card_number);
}

void data::invoice::set_order_number(const std::string& _number)
{
        if (!_number.empty() && (_number.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::ORDER_NUMBER);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->order_number = std::move(_number);
        }
        else
        {
                clear_flag(FLAG::ORDER_NUMBER);
        }
}

std::string data::invoice::get_order_number() const
{
        return std::move(this->order_number);
}

void data::invoice::set_description_total(const std::string& _total)
{
        if (!_total.empty() && (_total.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::DESCRIPTION_TOTAL);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->description_total = std::move(_total);
        }
        else
        {
                clear_flag(FLAG::DESCRIPTION_TOTAL);
        }
}

std::string data::invoice::get_description_total() const
{
        return std::move(this->description_total);
}

void data::invoice::set_material_total(const std::string& _total)
{
        if (!_total.empty() && (_total.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::MATERIAL_TOTAL);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->material_total = std::move(_total);
        }
        else
        {
                clear_flag(FLAG::MATERIAL_TOTAL);
        }
}

std::string data::invoice::get_material_total() const
{
        return std::move(this->material_total);
}

void data::invoice::set_grand_total(const std::string& _total)
{
        if (!_total.empty() && (_total.length() <= upper_bound::string_length))
        {
                set_flag(FLAG::GRAND_TOTAL);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->grand_total = std::move(_total);
        }
        else
        {
                clear_flag(FLAG::GRAND_TOTAL);
        }
}

std::string data::invoice::get_grand_total() const
{
        return std::move(this->grand_total);
}

void data::invoice::set_description_column(const std::vector<data::column>& _column)
{
        if (!_column.empty())
        {
                set_flag(FLAG::DESCRIPTION_COL);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->description_column = std::move(_column);
        }
        else
        {
                clear_flag(FLAG::DESCRIPTION_COL);
        }
}

std::vector<data::column> data::invoice::get_description_column() const
{
        return std::move(this->description_column);
}

void data::invoice::set_material_column(const std::vector<data::column>& _column)
{
        if (!_column.empty())
        {
                set_flag(FLAG::MATERIAL_COL);
                std::lock_guard<std::mutex> guard(this->invoice_data);
                this->material_column = std::move(_column);
        }
        else
        {
                clear_flag(FLAG::MATERIAL_COL);
        }
}

std::vector<data::column> data::invoice::get_material_column() const
{
        return std::move(this->material_column);
}

bool data::invoice::check_flags() const
{
        return ((this->flags & this->mask) == this->mask) ? true : false;
}

void data::invoice::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->invoice_data);
        this->flags |= static_cast<mask_type>(BIT::SET << bit);
}

void data::invoice::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->invoice_data);
        this->flags |= static_cast<mask_type>(BIT::UNSET << bit);
}
