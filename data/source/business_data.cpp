/********************************************************
 * Contents: business data implementation
 * Author: Dawid J. Blom
 * Date: October 13, 2025
 *
 * NOTE:
 *******************************************************/
#include <business_data.h>


namespace upper_bound {
        constexpr std::uint8_t MAX_ENTRY_LENGTH{50};
}

data::business::business(const business& _copy)
        : name{_copy.name}, address{_copy.address}, area_code{_copy.area_code},
          town{_copy.town}, cellphone{_copy.cellphone}, email_addresses{_copy.email_addresses},
          flags{_copy.flags}, business_data{}, mask{_copy.mask}
{
}

data::business::business(business&& _move)
        : name{std::move(_move.name)}, address{std::move(_move.address)}, area_code{std::move(_move.area_code)},
          town{std::move(_move.town)}, cellphone{std::move(_move.cellphone)}, email_addresses{std::move(_move.email_addresses)},
          flags{std::move(_move.flags)}, business_data{}, mask{std::move(_move.mask)}
{
        _move.name.clear();
        _move.address.clear();
        _move.area_code.clear();
        _move.town.clear();
        _move.cellphone.clear();
        _move.email_addresses.clear();
        _move.flags = 0;
        _move.mask = this->mask;
}

data::business& data::business::operator= (const business& _copy)
{
        business temp {_copy};
        std::swap(temp, *this);

        return *this;
}

data::business& data::business::operator= (business&& _move)
{
        std::swap(name, _move.name);
        std::swap(address, _move.address);
        std::swap(area_code, _move.area_code);
        std::swap(town, _move.town);
        std::swap(cellphone, _move.cellphone);
        std::swap(email_addresses, _move.email_addresses);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

bool data::business::is_valid() const
{
        return this->check_flags();
}

void data::business::set_name(const std::string& _name)
{
        if (!_name.empty() && (_name.length() <= upper_bound::MAX_ENTRY_LENGTH))
        {
                set_flag(FLAG::NAME);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->name = std::move(_name);
        }
        else
        {
                clear_flag(FLAG::NAME);
        }
}

std::string data::business::get_name() const
{
        return this->name;
}

void data::business::set_address(const std::string& _address)
{
        if (!_address.empty() && (_address.length() <= upper_bound::MAX_ENTRY_LENGTH))
        {
                set_flag(FLAG::ADDRESS);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->address = std::move(_address);
        }
        else
        {
                clear_flag(FLAG::ADDRESS);
        }
}

std::string data::business::get_address() const
{
        return this->address;
}

void data::business::set_area_code(const std::string& _code)
{
        if (!_code.empty() && (_code.length() <= upper_bound::MAX_ENTRY_LENGTH))
        {
                set_flag(FLAG::AREA_CODE);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->area_code = std::move(_code);
        }
        else
        {
                clear_flag(FLAG::AREA_CODE);
        }
}

std::string data::business::get_area_code() const
{
        return this->area_code;
}

void data::business::set_town(const std::string& _town)
{
        if (!_town.empty() && (_town.length() <= upper_bound::MAX_ENTRY_LENGTH))
        {
                set_flag(FLAG::TOWN);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->town = std::move(_town);
        }
        else
        {
                clear_flag(FLAG::TOWN);
        }
}

std::string data::business::get_town() const
{
        return this->town;
}

void data::business::set_cellphone(const std::string& _number)
{
        if (!_number.empty() && (_number.length() <= upper_bound::MAX_ENTRY_LENGTH))
        {
                set_flag(FLAG::CELLPHONE);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->cellphone = std::move(_number);
        }
        else
        {
                clear_flag(FLAG::CELLPHONE);
        }
}

std::string data::business::get_cellphone() const
{
        return this->cellphone;
}

void data::business::set_email(const std::string& _email_addresses)
{
        utility::word_slicer slicer{};
        std::vector<std::string> sliced_emails{slicer.slice(_email_addresses)};
        if (email_address_good(sliced_emails) == true && _email_addresses.empty() == false)
        {
                set_flag(FLAG::EMAIL);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->email_addresses = std::move(_email_addresses);
        }
        else
        {
                clear_flag(FLAG::EMAIL);
        }
}

std::string data::business::get_email() const
{
        return this->email_addresses;
}

bool data::business::check_flags() const
{
        return (((this->flags & this->mask) == this->mask) && ((this->flags & ~this->mask) == 0));
}

void data::business::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->business_data);
        this->flags |= static_cast<mask_type>(BIT::SET << bit);
}

void data::business::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->business_data);
        this->flags &= ~static_cast<mask_type> (BIT::CLEAR << bit);
}

bool data::business::email_address_good(const std::vector<std::string>& _email_addresses)
{
        std::regex email_address_regex(R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");
        for (const auto& email_address : _email_addresses)
        {
                bool email_address_format_correct{std::regex_search(email_address, email_address_regex)};
                if (!email_address_format_correct || !(email_address.length() <= upper_bound::MAX_ENTRY_LENGTH))
                {
                        return false;
                }
        }

        return true;
}
