#include <statement_data.h>

void data::statement::set_columns(const data::statement_column& _columns)
{
        if (_columns.is_valid())
        {
                this->columns = std::move(_columns);
        }
}

data::statement_column data::statement::get_columns() const
{
	return this->columns;
}
