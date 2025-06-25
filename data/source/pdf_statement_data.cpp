#include <pdf_statement_data.h>



data::pdf_statement::pdf_statement() {}

data::pdf_statement::~pdf_statement() {}

data::pdf_statement::pdf_statement(const pdf_statement& _copy)
	: statement{_copy.statement}, pdf_invoices{_copy.pdf_invoices},
	  data_mutex{}, flags{_copy.flags}, mask{_copy.mask}
{
}

data::pdf_statement::pdf_statement(pdf_statement&& _move)
	: statement{_move.statement}, pdf_invoices{_move.pdf_invoices},
	  data_mutex{}, flags{_move.flags}, mask{_move.mask}
{
        _move.statement = statement;
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
        std::swap(statement, _move.statement);
        std::swap(pdf_invoices, _move.pdf_invoices);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

bool data::pdf_statement::is_valid() const
{
	return check_flags();
}

void data::pdf_statement::set_statement(const data::statement& _statement)
{
        if (_statement.is_valid())
        {
                set_flag(FLAG::STATEMENT);
                std::lock_guard<std::mutex> guard(this->data_mutex);
                this->statement = std::move(_statement);
        }
        else
        {
                clear_flag(FLAG::STATEMENT);
	}
}

data::statement data::pdf_statement::get_statement() const
{
	return this->statement;
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
        return (((this->flags & this->mask) == this->mask) && ((this->flags & ~this->mask) == 0));
}
