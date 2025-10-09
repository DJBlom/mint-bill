/********************************************************
 * Contents: sqlite class declaration
 * Author: Dawid J. Blom
 * Date: November 25, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _SQLITE_H_
#define _SQLITE_H_
#include <string>
#include <vector>
#include <variant>
#include <sqlcipher/sqlite3.h>


namespace storage {
namespace database {
// Covers all 5 SQLite storage classes: NULL, INTEGER, REAL, TEXT, BLOB
using blob = std::vector<std::byte>;
using param_values = std::variant<std::nullptr_t, sqlite3_int64, double, std::string, blob>;

namespace part {
using column_value = param_values;
using row = std::vector<column_value>;
using rows = std::vector<row>;
}
class sqlite {
public:
	sqlite() = delete;
	explicit sqlite(const std::string&, const std::string&);
	sqlite(const sqlite&) = delete;
	sqlite(sqlite&&) = delete;
	sqlite& operator= (const sqlite&) = delete;
	sqlite& operator= (sqlite&&) = delete;
	virtual ~sqlite();

	[[nodiscard]] virtual bool transaction(const std::string&);
	[[nodiscard]] virtual bool usert(const std::string&, const std::vector<param_values>&);
	[[nodiscard]] virtual part::rows select(const std::string&, const std::vector<param_values>&);
	[[nodiscard]] virtual part::rows select(const std::string&);

private:
	sqlite3 *database{nullptr};
};

namespace part
{
class sql_operations {
public:
	sql_operations() = delete;
	explicit sql_operations(sqlite3*, const std::string&);
	sql_operations(const sql_operations&) = delete;
	sql_operations(sql_operations&&) = delete;
	sql_operations& operator= (const sql_operations&) = delete;
	sql_operations& operator= (sql_operations&&) = delete;
	virtual ~sql_operations();

	[[nodiscard]] bool virtual bind_params(const std::vector<param_values>&);
	[[nodiscard]] bool virtual single_execute();
	[[nodiscard]] rows virtual multi_execute();

private:
	[[nodiscard]] row collect_row_data();
	[[nodiscard]] column_value convert_column_value(const int&);

private:
	sqlite3_stmt *sql_stmt{nullptr};
};

struct binder {
	binder() = delete;
	binder(sqlite3_stmt *_sql_stmt, const int& _id_index): id_index{_id_index}, sql_stmt{_sql_stmt} {}
	binder(const binder&) = delete;
	binder(binder&&) = delete;
	binder& operator= (const binder&) = delete;
	binder& operator= (binder&&) = delete;
	~binder() = default;

	int operator()(std::nullptr_t) const;
	int operator()(const sqlite3_int64&) const;
	int operator()(const double&) const;
	int operator()(const std::string&) const;
	int operator()(const blob&) const;

	int id_index{0};
	sqlite3_stmt *sql_stmt{nullptr};
};
}
}
}
#endif
