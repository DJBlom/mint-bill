/*****************************************************************************
 * @file column_data.cpp
 *
 * @brief
 *   Implements the column data model defined in column_data.h, including
 *   construction, assignment, validation, and controlled updates to column
 *   fields.
 *
 * @details
 *   This implementation provides the behavior for data::column, including copy
 *   and move semantics as well as setter and getter methods for quantity,
 *   description, amount, row number, and the is_description flag. Setter
 *   functions apply basic validation rules, such as length constraints for
 *   formatted numeric and textual data, and range checks for numeric fields.
 *
 *   The amount field is formatted with fixed precision before length
 *   verification, ensuring that external constraints are applied to a
 *   predictable representation. A bitmask-based flag mechanism is used to
 *   track which attributes are initialized, and check_flags() consolidates
 *   this state so that is_valid() can report completeness. A mutex guards
 *   modifications to internal members, enabling thread-safe use in concurrent
 *   code.
 *
 * @notes
 *   Field limits are defined in the local limit namespace, making it easy to
 *   adjust constraints without changing the class interface. The logical
 *   is_description flag is validated against LOGICAL_TRUE and LOGICAL_FALSE,
 *   enabling clear separation of purely descriptive rows from rows that carry
 *   transactional values.
 *****************************************************************************/
#include <column_data.h>
#include <limits>


namespace limit {
        constexpr std::uint8_t MAX_QUANTITY{9};
        constexpr std::uint16_t MAX_DESCRIPTION{500};
        constexpr std::uint8_t MAX_AMOUNT{15};
}

data::column::column() {}

data::column::column(const column& _copy)
        : quantity{_copy.quantity}, description{_copy.description},
          amount{_copy.amount}, row_number{_copy.row_number},
	  is_description{_copy.is_description}, flags{_copy.flags},
	  column_data{}, mask{_copy.mask} {}

data::column::column(column&& _move)
        : quantity{std::move(_move.quantity)}, description{std::move(_move.description)},
          amount{std::move(_move.amount)}, row_number{std::move(_move.row_number)},
	  is_description{std::move(_move.is_description)}, flags{std::move(_move.flags)},
	  column_data{}, mask{std::move(_move.mask)}
{
        _move.quantity = 0;
        _move.description.clear();
        _move.amount = 0.0;
        _move.is_description = 0;
	_move.row_number = 0;
        _move.flags = 0;
        _move.mask = this->mask;
}

data::column& data::column::operator= (const column& _copy)
{
        column temp{_copy};
        std::swap(temp, *this);

        return *this;
}

data::column& data::column::operator= (column&& _move)
{
        std::swap(quantity, _move.quantity);
        std::swap(description, _move.description);
        std::swap(amount, _move.amount);
        std::swap(is_description, _move.is_description);
	std::swap(row_number, _move.row_number);
        std::swap(flags, _move.flags);
        std::swap(mask, _move.mask);

        return *this;
}

data::column::~column() {}

bool data::column::is_valid() const
{
        return this->check_flags();
}

void data::column::set_quantity(const unsigned int& _quantity)
{
        std::string tmp{std::to_string(_quantity)};
        if (tmp.length() <= limit::MAX_QUANTITY)
        {
                this->set_flag(FLAG::QUANTITY);
                std::lock_guard<std::mutex> guard(this->column_data);
                this->quantity = std::move(_quantity);
        }
        else
        {
                this->clear_flag(FLAG::QUANTITY);
        }
}

unsigned int data::column::get_quantity() const
{
        return this->quantity;
}

void data::column::set_description(const std::string& _description)
{
        if (!_description.empty() && (_description.length() <= limit::MAX_DESCRIPTION))
        {
                this->set_flag(FLAG::DESCRIPTION);
                std::lock_guard<std::mutex> guard(this->column_data);
                this->description = std::move(_description);
        }
        else
        {
                this->clear_flag(FLAG::DESCRIPTION);
        }
}

std::string data::column::get_description() const
{
        return this->description;
}

void data::column::set_amount(const double& _amount)
{
        std::ostringstream amss{};
        amss.imbue(std::locale::classic());
        amss << std::fixed << std::setprecision(2) << _amount;
        std::string tmp{amss.str()};
        if (tmp.length() <= limit::MAX_AMOUNT)
        {
                this->set_flag(FLAG::AMOUNT);
                std::lock_guard<std::mutex> guard(this->column_data);
                this->amount = std::move(_amount);
        }
        else
        {
                this->clear_flag(FLAG::AMOUNT);
        }
}

double data::column::get_amount() const
{
        return this->amount;
}

void data::column::set_row_number(const long long& _row_number)
{
        if (_row_number < std::numeric_limits<long long>::max() && _row_number > std::numeric_limits<long long>::min())
        {
                this->set_flag(FLAG::ROW_NUMBER);
                std::lock_guard<std::mutex> guard(this->column_data);
                this->row_number = std::move(_row_number);
        }
        else
        {
                this->clear_flag(FLAG::ROW_NUMBER);
        }
}

long long data::column::get_row_number() const
{
        return this->row_number;
}

void data::column::set_is_description(const long long& _is_description)
{
        if (_is_description == LOGICAL_FALSE || _is_description == LOGICAL_TRUE)
        {
                this->set_flag(FLAG::IS_DESCRIPTION);
                std::lock_guard<std::mutex> guard(this->column_data);
                this->is_description = std::move(_is_description);
        }
        else
        {
                this->clear_flag(FLAG::IS_DESCRIPTION);
        }
}

long long data::column::get_is_description() const
{
        return this->is_description;
}

bool data::column::check_flags() const
{
        return (((this->flags & this->mask) == this->mask) && ((this->flags & ~this->mask) == 0));
}

void data::column::set_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->column_data);
        this->flags |= static_cast<mask_type>(BIT::SET << bit);
}

void data::column::clear_flag(const int& bit)
{
        std::lock_guard<std::mutex> guard(this->column_data);
        this->flags &= ~static_cast<mask_type> (BIT::CLEAR << bit);
}
