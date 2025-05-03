/********************************************************
 * Contents: Email data implementation
 * Author: Dawid J. Blom
 * Date: December 30, 2024
 *
 * NOTE:
 *******************************************************/
#include <email_data.h>


namespace limits {
        constexpr unsigned int max{50};
}

data::email::email() {}

data::email::email(const email& _copy)
        : attachments{_copy.attachments}, client{_copy.client}, business{_copy.business},
          subject{_copy.subject}, flags{_copy.flags}, email_data{}, mask{_copy.mask}
{
}

data::email::email(email&& _move)
        : attachments{_move.attachments}, client{_move.client}, business{_move.business},
          subject{_move.subject}, flags{_move.flags}, email_data{}, mask{_move.mask}
{
        _move.attachments.clear();
        _move.client = client;
        _move.business = business;
        _move.subject.clear();
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
        std::swap(attachments, _move.attachments);
        std::swap(client, _move.client);
        std::swap(business, _move.business);
        std::swap(subject, _move.subject);
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

void data::email::set_attachments(const std::vector<std::string>& _attachments)
{
        if (!_attachments.empty())
        {
                set_flag(FLAG::PDF);
                std::lock_guard<std::mutex> guard(this->email_data);
                this->attachments = std::move(_attachments);
        }
        else
        {
                clear_flag(FLAG::PDF);
        }
}

std::vector<std::string> data::email::get_attachments() const
{
        return std::move(this->attachments);
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

void data::email::set_subject(const std::string& _subject)
{
        if (!_subject.empty() && _subject.length() <= limits::max)
        {
                set_flag(FLAG::SUBJECT);
                std::lock_guard<std::mutex> guard(this->email_data);
                this->subject = std::move(_subject);
        }
        else
        {
                clear_flag(FLAG::SUBJECT);
        }
}

std::string data::email::get_subject() const
{
        return std::move(this->subject);
}

bool data::email::check_flags() const
{
        return (((this->flags & this->mask) == this->mask) && ((this->flags & ~this->mask) == 0));
}

void data::email::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->email_data);
        this->flags |= static_cast<mask_type>(BIT::SET << bit);
}

void data::email::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->email_data);
        this->flags &= ~static_cast<mask_type> (BIT::CLEAR << bit);
}
