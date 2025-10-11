#include <pdf_statement_data.h>



data::pdf_statement::pdf_statement() {}

data::pdf_statement::~pdf_statement() {}

data::pdf_statement::pdf_statement(const pdf_statement& _copy)
	: number{_copy.number}, date{_copy.date}, total{_copy.total},
	  pdf_invoices{_copy.pdf_invoices}, data_mutex{},
	  flags{_copy.flags}, mask{_copy.mask}
{
}

data::pdf_statement::pdf_statement(pdf_statement&& _move)
	: number{std::move(_move.number)}, date{std::move(_move.date)},
	  total{std::move(_move.total)}, pdf_invoices{std::move(_move.pdf_invoices)},
	  data_mutex{}, flags{std::move(_move.flags)}, mask{std::move(_move.mask)}
{
        _move.number = number;
        _move.date = date;
        _move.total = total;
        _move.pdf_invoices = pdf_invoices;
        _move.flags = 0;
        _move.mask = this->mask;
}

data::pdf_statement& data::pdf_statement::operator= (const pdf_statement& _copy)
{
        pdf_statement temp{_copy};
        std::swap(temp, *this);

        return *this;
}

data::pdf_statement& data::pdf_statement::operator= (pdf_statement&& _move)
{
        std::swap(number, _move.number);
        std::swap(date, _move.date);
        std::swap(total, _move.total);
        std::swap(pdf_invoices, _move.pdf_invoices);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

bool data::pdf_statement::is_valid() const
{
	return check_flags();
}

void data::pdf_statement::set_number(const std::string& _number)
{

        if (!_number.empty())
        {
                set_flag(FLAG::NUMBER);
                std::lock_guard<std::mutex> guard(this->data_mutex);
                this->number = std::move(_number);
        }
        else
        {
                clear_flag(FLAG::NUMBER);
	}
}

std::string data::pdf_statement::get_number() const
{
	return this->number;
}

void data::pdf_statement::set_date(const std::string& _date)
{
        if (!_date.empty())
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

std::string data::pdf_statement::get_date() const
{
	return this->date;
}

void data::pdf_statement::set_total(const std::string& _total)
{

        if (!_total.empty())
        {
                set_flag(FLAG::TOTAL);
                std::lock_guard<std::mutex> guard(this->data_mutex);
                this->total = std::move(_total);
        }
        else
        {
                clear_flag(FLAG::TOTAL);
	}
}

std::string data::pdf_statement::get_total() const
{
	return this->total;
}

void data::pdf_statement::set_pdf_invoices(const std::vector<data::pdf_invoice>& _pdf_invoices)
{
	if (!_pdf_invoices.empty())
	{
                set_flag(FLAG::PDF_INVOICE);
                std::lock_guard<std::mutex> guard(this->data_mutex);
		this->pdf_invoices = std::move(_pdf_invoices);
	}
        else
        {
                clear_flag(FLAG::PDF_INVOICE);
	}
}

std::vector<data::pdf_invoice> data::pdf_statement::get_pdf_invoices() const
{
	return this->pdf_invoices;
}

void data::pdf_statement::set_flag(const int& _bit)
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        this->flags |= static_cast<mask_type>(BIT::SET << _bit);
}

void data::pdf_statement::clear_flag(const int& _bit)
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        this->flags &= ~static_cast<mask_type> (BIT::CLEAR << _bit);
}

bool data::pdf_statement::check_flags() const
{
        return ((this->flags & this->mask) == this->mask);
}
