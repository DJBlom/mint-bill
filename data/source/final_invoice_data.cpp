/********************************************************
 * Contents: Final invoice data implementation
 * Author: Dawid J. Blom
 * Date: December 16, 2024
 *
 * NOTE:
 *******************************************************/
#include <final_invoice_data.h>


data::final_invoice::final_invoice() {}
data::final_invoice::final_invoice(const final_invoice& data)
        : client_name{data.client_name}, client_address{data.client_address},
          client_town{data.client_town}, client_vat_number{data.client_vat_number},
          invoice_number{data.invoice_number}, invoice_order_number{data.invoice_order_number},
          invoice_job_card{data.invoice_job_card}, invoice_date{data.invoice_date},
          description_column{data.description_column}, material_column{data.material_column},
          invoice_description_total{data.invoice_description_total},
          invoice_material_total{data.invoice_material_total}, invoice_grand_total{data.invoice_grand_total},
          business_name{data.business_name}, business_address{data.business_address},
          business_town{data.business_town}, business_cellphone{data.business_cellphone},
          business_email{data.business_email}, business_bank_name{data.business_bank_name},
          business_branch_code{data.business_branch_code}, business_account_number{data.business_account_number},
          business_message{data.business_message}, flags{data.flags}, final_invoice_data{}, mask{data.mask} {}

data::final_invoice::final_invoice(final_invoice&& data)
        : client_name{data.client_name}, client_address{data.client_address},
          client_town{data.client_town}, client_vat_number{data.client_vat_number},
          invoice_number{data.invoice_number}, invoice_order_number{data.invoice_order_number},
          invoice_job_card{data.invoice_job_card}, invoice_date{data.invoice_date},
          description_column{data.description_column}, material_column{data.material_column},
          invoice_description_total{data.invoice_description_total},
          invoice_material_total{data.invoice_material_total}, invoice_grand_total{data.invoice_grand_total},
          business_name{data.business_name}, business_address{data.business_address},
          business_town{data.business_town}, business_cellphone{data.business_cellphone},
          business_email{data.business_email}, business_bank_name{data.business_bank_name},
          business_branch_code{data.business_branch_code}, business_account_number{data.business_account_number},
          business_message{data.business_message}, flags{data.flags}, final_invoice_data{}, mask{data.mask}
{
        data.client_name.clear();
        data.client_address.clear();
        data.client_town.clear();
        data.client_vat_number.clear(),
        data.invoice_number.clear();
        data.invoice_order_number.clear();
        data.invoice_job_card.clear();
        data.invoice_date.clear(),
        data.description_column.clear();
        data.material_column.clear();
        data.invoice_description_total.clear();
        data.invoice_material_total.clear();
        data.invoice_grand_total.clear();
        data.business_name.clear();
        data.business_address.clear(),
        data.business_town.clear();
        data.business_cellphone.clear(),
        data.business_email.clear();
        data.business_bank_name.clear();
        data.business_branch_code.clear();
        data.business_account_number.clear();
        data.business_message.clear();
        data.flags = 0x0;
        data.mask = 0x7;
}

data::final_invoice& data::final_invoice::operator= (const final_invoice& data)
{
        final_invoice temp{data};
        std::swap(temp, *this);

        return *this;
}

data::final_invoice& data::final_invoice::operator= (final_invoice&& data)
{
        std::swap(client_name, data.client_name);
        std::swap(client_address, data.client_address);
        std::swap(client_town, data.client_town);
        std::swap(client_vat_number, data.client_vat_number);
        std::swap(invoice_number, data.invoice_number);
        std::swap(invoice_order_number, data.invoice_order_number);
        std::swap(invoice_job_card, data.invoice_job_card);
        std::swap(invoice_date, data.invoice_date);
        std::swap(description_column, data.description_column);
        std::swap(material_column, data.material_column);
        std::swap(invoice_description_total, data.invoice_description_total);
        std::swap(invoice_material_total, data.invoice_material_total);
        std::swap(invoice_grand_total, data.invoice_grand_total);
        std::swap(business_name, data.business_name);
        std::swap(business_address, data.business_address);
        std::swap(business_town, data.business_town);
        std::swap(business_cellphone, data.business_cellphone);
        std::swap(business_email, data.business_email);
        std::swap(business_bank_name, data.business_bank_name);
        std::swap(business_branch_code, data.business_branch_code);
        std::swap(business_account_number, data.business_account_number);
        std::swap(business_message, data.business_message);
        std::swap(flags, data.flags);
        std::swap(mask, data.mask);

        return *this;
}

data::final_invoice::~final_invoice() {}

bool data::final_invoice::is_valid() const
{
        bool is_valid{false};
        if (this->check_flags() == true)
        {
                is_valid = true;
        }

        return is_valid;
}

void data::final_invoice::update_client_data(const data::client& data)
{
        if (data.is_valid() == true)
        {
                set_flag(FLAG::CLIENT_DATA);
                std::lock_guard<std::mutex> guard(this->final_invoice_data);
                this->client_name = data.get_business_name();
                this->client_address = data.get_business_address();
                this->client_town = data.get_business_town_name();
                this->client_vat_number = data.get_vat_number();
        }
        else
        {
                clear_flag(FLAG::CLIENT_DATA);
        }
}

void data::final_invoice::update_invoice_data(const data::invoice& data)
{
        if (data.is_valid() == true)
        {
                set_flag(FLAG::INVOICE_DATA);
                std::lock_guard<std::mutex> guard(this->final_invoice_data);
                this->invoice_number = std::to_string(data.get_invoice_number());
                this->invoice_order_number = data.get_order_number();
                this->invoice_job_card = data.get_job_card_number();
                this->invoice_date = data.get_invoice_date();
                this->description_column = data.get_description_column();
                this->material_column = data.get_material_column();
                this->invoice_description_total = data.get_description_total();
                this->invoice_material_total = data.get_material_total();
                this->invoice_grand_total = data.get_grand_total();
        }
        else
        {
                clear_flag(FLAG::INVOICE_DATA);
        }
}

void data::final_invoice::update_business_data(const data::business& data)
{
        if (data.is_valid() == true)
        {
                set_flag(FLAG::BUSINESS_DATA);
                std::lock_guard<std::mutex> guard(this->final_invoice_data);
                this->business_name = data.get_name();
                this->business_address = data.get_address();
                this->business_town = data.get_town();
                this->business_cellphone = data.get_cellphone();
                this->business_email = data.get_email();
                this->business_bank_name = data.get_bank();
                this->business_branch_code = data.get_branch_code();
                this->business_account_number = data.get_account_number();
                this->business_message = data.get_client_message();
        }
        else
        {
                clear_flag(FLAG::BUSINESS_DATA);
        }
}

bool data::final_invoice::check_flags() const
{
        return ((this->flags & this->mask) == 0x7) ? true : false;
}

void data::final_invoice::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->final_invoice_data);
        this->flags |= static_cast<std::uint8_t>(BIT::SET << bit);
}

void data::final_invoice::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->final_invoice_data);
        this->flags |= static_cast<std::uint8_t>(BIT::UNSET << bit);
}
