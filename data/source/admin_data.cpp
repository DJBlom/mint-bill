/********************************************************
 * Contents: admin data implementation
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#include <admin_data.h>


namespace upper_bound {
        constexpr std::uint8_t MAX_ENTRY_LENGTH{50};
        constexpr std::uint8_t MESSAGE_LENGTH{100};
        constexpr std::uint8_t PASSWORD_LENGTH{100};
}

data::admin::admin(const admin& _copy)
        : name{_copy.name}, address{_copy.address}, area_code{_copy.area_code},
          town{_copy.town}, cellphone{_copy.cellphone}, email{_copy.email},
          bank{_copy.bank}, branch_code{_copy.branch_code}, account_number{_copy.account_number},
          client_message{_copy.client_message}, password{_copy.password}, flags{_copy.flags},
          business_data{}, mask{_copy.mask}
{
}

data::admin::admin(admin&& _move)
        : name{std::move(_move.name)}, address{std::move(_move.address)}, area_code{std::move(_move.area_code)},
          town{std::move(_move.town)}, cellphone{std::move(_move.cellphone)}, email{std::move(_move.email)},
          bank{std::move(_move.bank)}, branch_code{std::move(_move.branch_code)}, account_number{std::move(_move.account_number)},
          client_message{std::move(_move.client_message)}, password{std::move(_move.password)}, flags{std::move(_move.flags)},
          business_data{}, mask{std::move(_move.mask)}
{
        _move.name.clear();
        _move.address.clear();
        _move.area_code.clear();
        _move.town.clear();
        _move.cellphone.clear();
        _move.email.clear();
        _move.bank.clear();
        _move.branch_code.clear();
        _move.account_number.clear();
        _move.client_message.clear();
        _move.password.clear();
        _move.flags = 0;
        _move.mask = this->mask;
}

data::admin& data::admin::operator= (const admin& _copy)
{
        admin temp {_copy};
        std::swap(temp, *this);

        return *this;
}

data::admin& data::admin::operator= (admin&& _move)
{
        std::swap(name, _move.name);
        std::swap(address, _move.address);
        std::swap(area_code, _move.area_code);
        std::swap(town, _move.town);
        std::swap(cellphone, _move.cellphone);
        std::swap(email, _move.email);
        std::swap(bank, _move.bank);
        std::swap(branch_code, _move.branch_code);
        std::swap(account_number, _move.account_number);
        std::swap(client_message, _move.client_message);
        std::swap(password, _move.password);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

bool data::admin::is_valid() const
{
        return this->check_flags();
}

void data::admin::set_name(const std::string& _name)
{
        if (!_name.empty() && (_name.length() <= upper_bound::MAX_ENTRY_LENGTH))
        {
                set_flag(FLAG::NAME);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->name = _name;
        }
        else
        {
                clear_flag(FLAG::NAME);
        }
}

std::string data::admin::get_name() const
{
        return this->name;
}

void data::admin::set_address(const std::string& _address)
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

std::string data::admin::get_address() const
{
        return this->address;
}

void data::admin::set_area_code(const std::string& _code)
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

std::string data::admin::get_area_code() const
{
        return this->area_code;
}

void data::admin::set_town(const std::string& _town)
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

std::string data::admin::get_town() const
{
        return this->town;
}

void data::admin::set_cellphone(const std::string& _number)
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

std::string data::admin::get_cellphone() const
{
        return this->cellphone;
}

void data::admin::set_email(const std::string& _email)
{
        std::regex email_regex(R"((^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$))");
        bool correct_email_format{std::regex_search(_email, email_regex)};
        if (correct_email_format && (_email.length() <= upper_bound::MAX_ENTRY_LENGTH))
        {
                set_flag(FLAG::EMAIL);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->email = std::move(_email);
        }
        else
        {
                clear_flag(FLAG::EMAIL);
        }
}

std::string data::admin::get_email() const
{
        return this->email;
}

void data::admin::set_bank(const std::string& _bank)
{
        if (!_bank.empty() && (_bank.length() <= upper_bound::MAX_ENTRY_LENGTH))
        {
                set_flag(FLAG::BANK);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->bank = std::move(_bank);
        }
        else
        {
                clear_flag(FLAG::BANK);
        }
}

std::string data::admin::get_bank() const
{
        return this->bank;
}

void data::admin::set_branch_code(const std::string& _branch_number)
{
        if (!_branch_number.empty() && (_branch_number.length() <= upper_bound::MAX_ENTRY_LENGTH))
        {
                set_flag(FLAG::BRANCH_CODE);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->branch_code = std::move(_branch_number);
        }
        else
        {
                clear_flag(FLAG::BRANCH_CODE);
        }
}

std::string data::admin::get_branch_code() const
{
        return this->branch_code;
}

void data::admin::set_account_number(const std::string& _account_number)
{
        if (!_account_number.empty() && (_account_number.length() <= upper_bound::MAX_ENTRY_LENGTH))
        {
                set_flag(FLAG::ACCOUNT_NUMBER);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->account_number = std::move(_account_number);
        }
        else
        {
                clear_flag(FLAG::ACCOUNT_NUMBER);
        }
}

std::string data::admin::get_account_number() const
{
        return this->account_number;
}

void data::admin::set_client_message(const std::string& _message)
{
        if (!_message.empty() && (_message.length() <= upper_bound::MESSAGE_LENGTH))
        {
                set_flag(FLAG::CLIENT_MESSAGE);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->client_message = std::move(_message);
        }
        else
        {
                clear_flag(FLAG::CLIENT_MESSAGE);
        }
}

std::string data::admin::get_client_message() const
{
        return this->client_message;
}

void data::admin::set_password(const std::string& _password)
{
        if (_password.length() <= upper_bound::PASSWORD_LENGTH)
        {
                set_flag(FLAG::PASSWORD);
                std::lock_guard<std::mutex> guard(this->business_data);
                this->password = std::move(_password);
        }
        else
        {
                clear_flag(FLAG::PASSWORD);
        }
}

std::string data::admin::get_password() const
{
        return this->password;
}

bool data::admin::check_flags() const
{
        return (((this->flags & this->mask) == this->mask) && ((this->flags & ~this->mask) == 0));
}

void data::admin::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->business_data);
        this->flags |= static_cast<mask_type>(BIT::SET << bit);
}

void data::admin::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->business_data);
        this->flags &= ~static_cast<mask_type> (BIT::CLEAR << bit);
}
