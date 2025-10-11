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
        : attachments{_copy.attachments}, client{_copy.client}, admin{_copy.admin},
          subject{_copy.subject}, flags{_copy.flags}, email_data{}, mask{_copy.mask}
{
}

data::email::email(email&& _move)
        : attachments{std::move(_move.attachments)}, client{std::move(_move.client)},
	  admin{std::move(_move.admin)}, subject{std::move(_move.subject)},
	  flags{std::move(_move.flags)}, email_data{}, mask{std::move(_move.mask)}
{
        _move.attachments.clear();
        _move.client = client;
        _move.admin = admin;
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
        std::swap(admin, _move.admin);
        std::swap(subject, _move.subject);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

data::email::~email(){}

bool data::email::is_valid() const
{
        return this->check_flags();
}

void data::email::set_attachments(const std::vector<std::string>& _attachments)
{
        if (!_attachments.empty())
        {
                set_flag(FLAG::PDF);
                std::lock_guard<std::mutex> guard(this->email_data);
                this->attachments = _attachments;
        }
        else
        {
                clear_flag(FLAG::PDF);
        }
}

std::vector<std::string> data::email::get_attachments() const
{
        return this->attachments;
}

void data::email::set_client(const data::client& _client)
{
        if (_client.is_valid())
        {
                set_flag(FLAG::CLIENT);
                std::lock_guard<std::mutex> guard(this->email_data);
                this->client = _client;
        }
        else
        {
                clear_flag(FLAG::CLIENT);
        }
}

data::client data::email::get_client() const
{
        return this->client;
}

void data::email::set_business(const data::admin& _business)
{
        if (_business.is_valid())
        {
                set_flag(FLAG::ADMIN);
                std::lock_guard<std::mutex> guard(this->email_data);
                this->admin = _business;
        }
        else
        {
                clear_flag(FLAG::ADMIN);
        }
}

data::admin data::email::get_business() const
{
        return this->admin;
}

void data::email::set_subject(const std::string& _subject)
{
        if (!_subject.empty() && _subject.length() <= limits::max)
        {
                set_flag(FLAG::SUBJECT);
                std::lock_guard<std::mutex> guard(this->email_data);
                this->subject = _subject;
        }
        else
        {
                clear_flag(FLAG::SUBJECT);
        }
}

std::string data::email::get_subject() const
{
        return this->subject;
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
