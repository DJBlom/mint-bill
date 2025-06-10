/********************************************************
 * Contents: Statement data definition
 * Author: Dawid J. Blom
 * Date: February 19, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _STATEMENT_DATA_H_
#define _STATEMENT_DATA_H_
#include <statement_column.h>

namespace data {
struct statement {
public:
        statement() = default;
        statement(const statement&);
        statement(statement&&);
        statement& operator= (const statement&);
        statement& operator= (statement&&);
        virtual ~statement() = default;

	virtual void set_columns(const data::statement_column&);
	[[nodiscard]] virtual data::statement_column get_columns() const;

private:
	data::statement_column columns{};
};
}
#endif
