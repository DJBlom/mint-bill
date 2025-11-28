/********************************************************
 * Contents: Invoice data implementation
 * Author: Dawid J. Blom
 * Date: December 4, 2024
 *
 * NOTE:
 *******************************************************/
#include <invoice_data.h>


namespace upper_bound {
        constexpr std::uint8_t name_length{50};
        constexpr std::uint8_t string_length{20};
        constexpr std::uint8_t paid_status_length{10};
        constexpr std::uint8_t invoice_number_length{8};
}

data::invoice::invoice() {}

data::invoice::~invoice() {}

data::invoice::invoice(const invoice& _copy)
        : data::billing{_copy}, job_card_number{_copy.job_card_number}, order_number{_copy.order_number},
          description_total{_copy.description_total}, material_total{_copy.material_total},
          grand_total{_copy.grand_total}, description_column{_copy.description_column},
          material_column{_copy.material_column}, flags{_copy.flags}, invoice_data{},
          mask{_copy.mask} {}

data::invoice::invoice(invoice&& _move)
        : data::billing{std::move(_move)}, job_card_number{std::move(_move.job_card_number)},
	  order_number{std::move(_move.order_number)}, description_total{std::move(_move.description_total)},
	  material_total{std::move(_move.material_total)}, grand_total{std::move(_move.grand_total)},
	  description_column{std::move(_move.description_column)}, material_column{std::move(_move.material_column)},
	  flags{std::move(_move.flags)}, invoice_data{}, mask{std::move(_move.mask)}
{
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
	data::billing::operator=(_copy);

        return *this;
}

data::invoice& data::invoice::operator= (invoice&& _move)
{
	data::billing::operator=(_move);
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
        return this->check_flags() && data::billing::is_valid();
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
        return this->job_card_number;
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
        return this->order_number;
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
        return this->description_total;
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
        return this->material_total;
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
        return this->grand_total;
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
        return this->description_column;
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
        return this->material_column;
}

bool data::invoice::check_flags() const
{
        return (((this->flags & this->mask) == this->mask) && ((this->flags & ~this->mask) == 0));
}

void data::invoice::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->invoice_data);
        this->flags |= static_cast<mask_type>(BIT::SET << bit);
}

void data::invoice::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->invoice_data);
        this->flags &= ~static_cast<mask_type> (BIT::CLEAR << bit);
}
