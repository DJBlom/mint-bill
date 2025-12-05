/*****************************************************************************
 * @file admin_data.cpp
 *
 * @brief
 *   Implements the administrative data model defined in admin_data.h,
 *   providing construction, assignment, validation, and controlled access
 *   to administrative attributes associated with a business entity.
 *
 * @details
 *   This implementation defines the behavior of the data::admin class,
 *   including copy and move semantics, field setters and getters, and
 *   internal validity checks. Administrative attributes such as bank name,
 *   branch code, account number, client message, and password are stored
 *   and updated with basic length-based validation.
 *
 *   A bitmask-based flag mechanism is used to track which fields have been
 *   populated, enabling a concise implementation of is_valid() that combines
 *   the flag state with the base data::business validation logic. A mutex
 *   protects access to mutable internal data to support safe concurrent use
 *   from multiple threads.
 *
 * @notes
 *   The upper_bound namespace provides local compile-time limits for the
 *   maximum length of administrative string fields. Callers should rely on
 *   is_valid() rather than assuming that individual setters always succeed.
 *****************************************************************************/
#include <admin_data.h>


namespace upper_bound {
        constexpr std::uint8_t MAX_ENTRY_LENGTH{50};
        constexpr std::uint8_t MESSAGE_LENGTH{100};
        constexpr std::uint8_t PASSWORD_LENGTH{100};
}

data::admin::~admin() {}

data::admin::admin(const admin& _copy)
	: data::business{_copy}, bank{_copy.bank}, branch_code{_copy.branch_code},
	  account_number{_copy.account_number}, client_message{_copy.client_message},
	  password{_copy.password}, flags{_copy.flags}, business_data{}, mask{_copy.mask}
{
}

data::admin::admin(admin&& _move)
	: data::business{std::move(_move)}, bank{std::move(_move.bank)}, branch_code{std::move(_move.branch_code)},
	  account_number{std::move(_move.account_number)}, client_message{std::move(_move.client_message)},
	  password{std::move(_move.password)}, flags{std::move(_move.flags)}, business_data{}, mask{std::move(_move.mask)}
{
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
	data::business::operator=(_copy);

        return *this;
}

data::admin& data::admin::operator= (admin&& _move)
{
	data::business::operator=(std::move(_move));
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
        return this->check_flags() && data::business::is_valid();
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
