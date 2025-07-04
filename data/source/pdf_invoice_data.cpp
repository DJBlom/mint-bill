/********************************************************
 * Contents: PDF invoice data implementation
 * Author: Dawid J. Blom
 * Date: December 23, 2024
 *
 * NOTE:
 *******************************************************/
#include <pdf_invoice_data.h>


data::pdf_invoice::pdf_invoice() {}

data::pdf_invoice::~pdf_invoice() {}

data::pdf_invoice::pdf_invoice(const pdf_invoice& _copy)
        : client{_copy.client}, invoice{_copy.invoice},
          business{_copy.business}, flags{_copy.flags},
          data_mutex{}, mask{_copy.mask}
{
}

data::pdf_invoice::pdf_invoice(pdf_invoice&& _move)
        : client{std::move(_move.client)}, invoice{std::move(_move.invoice)},
          business{std::move(_move.business)}, flags{std::move(_move.flags)},
          data_mutex{}, mask{std::move(_move.mask)}
{
        _move.client = client;
        _move.invoice = invoice;
        _move.business = business;
        _move.flags = 0;
        _move.mask = this->mask;
}

data::pdf_invoice& data::pdf_invoice::operator= (const pdf_invoice& _copy)
{
        pdf_invoice temp{_copy};
        std::swap(temp, *this);

        return *this;
}

data::pdf_invoice& data::pdf_invoice::operator= (pdf_invoice&& _move)
{
        std::swap(client, _move.client);
        std::swap(invoice, _move.invoice);
        std::swap(business, _move.business);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

bool data::pdf_invoice::is_valid() const
{
        return this->check_flags();
}

void data::pdf_invoice::set_client(const data::client& _client)
{
        if (_client.is_valid() == true)
        {
                set_flag(FLAG::CLIENT);
                std::lock_guard<std::mutex> guard(this->data_mutex);
                this->client = std::move(_client);
        }
        else
        {
                clear_flag(FLAG::CLIENT);
        }
}

data::client data::pdf_invoice::get_client() const
{
        return this->client;
}

void data::pdf_invoice::set_invoice(const data::invoice& _invoice)
{
        if (_invoice.is_valid() == true)
        {
                set_flag(FLAG::INVOICE);
                std::lock_guard<std::mutex> guard(this->data_mutex);
                this->invoice = std::move(_invoice);
        }
        else
        {
                clear_flag(FLAG::INVOICE);
        }
}

data::invoice data::pdf_invoice::get_invoice() const
{
        return this->invoice;
}

void data::pdf_invoice::set_business(const data::business& _business)
{
        if (_business.is_valid() == true)
        {
                set_flag(FLAG::BUSINESS);
                std::lock_guard<std::mutex> guard(this->data_mutex);
                this->business = std::move(_business);
        }
        else
        {
                clear_flag(FLAG::BUSINESS);
        }
}

data::business data::pdf_invoice::get_business() const
{
        return this->business;
}

bool data::pdf_invoice::check_flags() const
{
        return (((this->flags & this->mask) == this->mask) && ((this->flags & ~this->mask) == 0));
}

void data::pdf_invoice::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        this->flags |= static_cast<mask_type>(BIT::SET << bit);
}

void data::pdf_invoice::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        this->flags &= ~static_cast<mask_type> (BIT::CLEAR << bit);
}
