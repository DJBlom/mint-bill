/********************************************************
 * Contents: Email data implementation
 * Author: Dawid J. Blom
 * Date: December 30, 2024
 *
 * NOTE:
 *******************************************************/
#include <email_data.h>


data::email::email() {}

data::email::email(const email& _copy)
        : pdf{_copy.pdf}, client{_copy.client}, business{_copy.business},
          flags{_copy.flags}, email_data{}, mask{_copy.mask}
{
}

data::email::email(email&& _move)
        : pdf{_move.pdf}, client{_move.client}, business{_move.business},
          flags{_move.flags}, email_data{}, mask{_move.mask}
{
        _move.flags = 0;
        _move.mask = this->mask;
}

data::email& data::email::operator= (const email& _copy)
{
        email temp{_copy};
        std::swap(temp, *this);

        return *this;
}

data::email& data::email::operator= (email&& _move)
{
        std::swap(pdf, _move.pdf);
        std::swap(client, _move.client);
        std::swap(business, _move.business);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

data::email::~email(){}

bool data::email::is_valid() const
{
        bool is_valid{false};
        if (this->check_flags() == true)
        {
                is_valid = true;
        }

        return is_valid;
}

void data::email::set_pdf(const std::string& _pdf)
{
        if (!_pdf.empty())
        {
                set_flag(FLAG::PDF);
                std::lock_guard<std::mutex> guard(this->email_data);
                this->pdf = std::move(_pdf);
        }
        else
        {
                clear_flag(FLAG::PDF);
        }
}

std::string data::email::get_pdf() const
{
        return std::move(this->pdf);
}

void data::email::set_client(const data::client& _client)
{
        if (_client.is_valid())
        {
                set_flag(FLAG::CLIENT);
                std::lock_guard<std::mutex> guard(this->email_data);
                this->client = std::move(_client);
        }
        else
        {
                clear_flag(FLAG::CLIENT);
        }
}

data::client data::email::get_client() const
{
        return std::move(this->client);
}

void data::email::set_business(const data::business& _business)
{
        if (_business.is_valid())
        {
                set_flag(FLAG::BUSINESS);
                std::lock_guard<std::mutex> guard(this->email_data);
                this->business = std::move(_business);
        }
        else
        {
                clear_flag(FLAG::BUSINESS);
        }
}

data::business data::email::get_business() const
{
        return std::move(this->business);
}

void data::email::set_files(const std::vector<std::string>& _files)
{
        if (!_files.empty() && have_file_names(_files) == true)
        {
                set_flag(FLAG::FILES);
                std::lock_guard<std::mutex> guard(this->email_data);
                this->files = std::move(_files);
        }
        else
        {
                clear_flag(FLAG::FILES);
        }
}

std::vector<std::string> data::email::get_files() const
{
        return this->files;
}

bool data::email::have_file_names(const std::vector<std::string>& _names)
{
        for (const auto& name : _names)
        {
                if (name.empty() == true)
                {
                        return false;
                }
        }

        return true;
}

bool data::email::check_flags() const
{
        return ((this->flags & this->mask) == this->mask) ? true : false;
}

void data::email::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->email_data);
        this->flags |= static_cast<mask_type>(BIT::SET << bit);
}

void data::email::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->email_data);
        this->flags |= static_cast<mask_type>(BIT::UNSET << bit);
}
