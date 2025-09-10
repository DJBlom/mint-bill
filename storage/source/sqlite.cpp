/********************************************************
 * Contents: Storage class implementation
 * Author: Dawid J. Blom
 * Date: November 25, 2024
 *
 * NOTE:
 *******************************************************/
#include <numeric>
#include <cstring>
#include <errors.h>
#include <sqlite.h>
#include <syslog.h>


//GCOVR_EXCL_START
storage::database::sqlite::sqlite(const std::string& _path, const std::string& _db_password)
{
	if (_path.empty() == true || _db_password.empty() == true)
	{
		syslog(LOG_CRIT, "SQLITE: invalid parameters - "
				 "filename %s, line number %d", __FILE__, __LINE__);
		throw app::errors::construction;
	}
	else
	{
		if (sqlite3_open_v2(_path.c_str(), &this->database, (SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX), nullptr) != SQLITE_OK)
		{
			sqlite3_close_v2(this->database);
			this->database = nullptr;
			syslog(LOG_CRIT, "SQLITE: failed to open the database - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			throw app::errors::construction;
		}

		if (sqlite3_key(this->database, _db_password.data(), static_cast<int> (_db_password.size())) != SQLITE_OK)
		{
			sqlite3_close_v2(this->database);
			this->database = nullptr;
			syslog(LOG_CRIT, "SQLITE: failed to decrypt the database - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			throw app::errors::construction;
		}

		if (sqlite3_db_config(this->database, SQLITE_DBCONFIG_ENABLE_FKEY, 1, nullptr) != SQLITE_OK)
		{
			sqlite3_close_v2(this->database);
			this->database = nullptr;
			syslog(LOG_CRIT, "SQLITE: failed to configure the database - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			throw app::errors::construction;
		}

		if (sqlite3_exec(this->database, "PRAGMA journal_mode=WAL;",   nullptr, nullptr, nullptr) != SQLITE_OK)
		{
			sqlite3_close_v2(this->database);
			this->database = nullptr;
			syslog(LOG_CRIT, "SQLITE: failed to execute statement - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			throw app::errors::construction;
		}

		if (sqlite3_exec(this->database, "PRAGMA synchronous=NORMAL;", nullptr, nullptr, nullptr) != SQLITE_OK)
		{
			sqlite3_close_v2(this->database);
			this->database = nullptr;
			syslog(LOG_CRIT, "SQLITE: failed to execute statement - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			throw app::errors::construction;
		}

		if (sqlite3_exec(this->database, "PRAGMA busy_timeout=5000;",  nullptr, nullptr, nullptr) != SQLITE_OK)
		{
			sqlite3_close_v2(this->database);
			this->database = nullptr;
			syslog(LOG_CRIT, "SQLITE: failed to execute statement - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			throw app::errors::construction;
		}
	}
}

storage::database::sqlite::~sqlite()
{
	if (sqlite3_close_v2(this->database) != SQLITE_OK)
	{
                syslog(LOG_CRIT, "SQLITE: failed to close the database connection - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
}
//GCOVR_EXCL_STOP

bool storage::database::sqlite::usert(const std::string& _sql_query, const std::vector<param_values>& _sql_query_params)
{
	bool success{false};
	if (_sql_query.empty() || _sql_query_params.empty())
	{
                syslog(LOG_CRIT, "SQLITE: invalid parameters - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		part::sql_operations sql_operations{this->database, _sql_query};
		if (sql_operations.bind_params(_sql_query_params) == false)
		{
			syslog(LOG_CRIT, "SQLITE: failed to bind the parameters - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = sql_operations.single_execute();
		}
	}

	return success;
}

storage::database::part::rows storage::database::sqlite::select(const std::string& _sql_query, const std::vector<param_values>& _sql_query_params)
{
	part::rows rows;
	if (_sql_query.empty() || _sql_query_params.empty())
	{
                syslog(LOG_CRIT, "SQLITE: invalid parameters - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		part::sql_operations sql_operations{this->database, _sql_query};
		if (sql_operations.bind_params(_sql_query_params) == false)
		{
			syslog(LOG_CRIT, "SQLITE: failed to bind the parameters - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			rows = std::move(sql_operations.multi_execute());
		}
	}

	return rows;
} //GCOVR_EXCL_LINE


/*********************************************************************************************************
 * SQL_OPERATIONS
 ********************************************************************************************************/
storage::database::part::sql_operations::sql_operations(sqlite3* _db_conn, const std::string& _sql_query)
{
	if (_db_conn == nullptr || _sql_query.empty())
	{
		syslog(LOG_CRIT, "SQL_OPERATIONS: invalid parameters - "
				 "filename %s, line number %d", __FILE__, __LINE__);
		throw app::errors::construction;
	}
	else
	{
		if (sqlite3_prepare_v2(_db_conn, _sql_query.c_str(), -1, &this->sql_stmt, nullptr) != SQLITE_OK)
		{
			syslog(LOG_CRIT, "SQL_OPERATIONS: failed to prepare sql_stmt - "
					 "filename %s, line number %d", __FILE__, __LINE__);
			throw app::errors::construction;
		}
	}
}

storage::database::part::sql_operations::~sql_operations()
{
	if (sqlite3_finalize(this->sql_stmt) != SQLITE_OK)
	{
		syslog(LOG_CRIT, "SQL_OPERATIONS: failed to finalize sql_stmt - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		this->sql_stmt = nullptr;
	}
}

bool storage::database::part::sql_operations::bind_params(const std::vector<param_values>& _sql_query_params)
{
	bool success{false};
	if (_sql_query_params.empty() == true)
	{
		syslog(LOG_CRIT, "SQL_OPERATIONS: invalid parameters - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (sqlite3_bind_parameter_count(this->sql_stmt) != static_cast<int> (_sql_query_params.size()))
		{
			syslog(LOG_CRIT, "SQL_OPERATIONS: failed to bind the parameter count - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = true;
			int index{0};
			for (const param_values& value : _sql_query_params)
			{
				std::visit(binder{sql_stmt, ++index}, value);
			}
		}
	}

	return success;
}

bool storage::database::part::sql_operations::single_execute()
{
	int return_code{sqlite3_step(this->sql_stmt)};
	return (return_code == SQLITE_DONE && return_code != SQLITE_ROW);
}

storage::database::part::rows storage::database::part::sql_operations::multi_execute()
{
	rows rows;
	while (sqlite3_step(this->sql_stmt) == SQLITE_ROW)
	{
		rows.emplace_back(std::move(this->collect_row_data()));
	}

	return rows;
} //GCOVR_EXCL_LINE

storage::database::part::row storage::database::part::sql_operations::collect_row_data()
{
	row row;
	const int number_of_columns = sqlite3_column_count(this->sql_stmt);
	row.reserve(number_of_columns);
	for (int col = 0; col < number_of_columns; ++col)
	{
		row.emplace_back(std::move(convert_column_value(col)));
	}

	return row;
} //GCOVR_EXCL_LINE

storage::database::part::column_value storage::database::part::sql_operations::convert_column_value(const int& _col_index)
{
	column_value column_value;
	int column_type{sqlite3_column_type(this->sql_stmt, _col_index)};
	if (column_type == SQLITE_NULL)
	{
		column_value = nullptr;
	}
	else if (column_type == SQLITE_INTEGER)
	{
		column_value = static_cast<sqlite3_int64>(sqlite3_column_int64(this->sql_stmt, _col_index));
	}
	else if (column_type == SQLITE_FLOAT)
	{
		column_value = sqlite3_column_double(this->sql_stmt, _col_index);
	}
	else if (column_type == SQLITE_TEXT)
	{
		column_value = std::string(reinterpret_cast<const char*>(
					sqlite3_column_text(this->sql_stmt, _col_index)),
					sqlite3_column_bytes(this->sql_stmt, _col_index));
	}
	else if (column_type == SQLITE_BLOB)
	{
		int number_of_bytes = sqlite3_column_bytes(this->sql_stmt, _col_index);
		const void* bytes = sqlite3_column_blob(this->sql_stmt, _col_index);
		storage::database::blob blob;
		if (number_of_bytes > 0 && bytes != nullptr)
		{
			blob.resize(number_of_bytes);
			std::memcpy(blob.data(), bytes, number_of_bytes);
		}

		column_value = blob;
	}

	return column_value;
} //GCOVR_EXCL_LINE


/*************************************************
 * binder
 *************************************************/
int storage::database::part::binder::operator()(std::nullptr_t) const
{
	return sqlite3_bind_null(this->sql_stmt, this->id_index);
}

int storage::database::part::binder::operator()(const sqlite3_int64& _data) const
{
	return sqlite3_bind_int64(this->sql_stmt, this->id_index, _data);
}

int storage::database::part::binder::operator()(const double& _data) const
{
	return sqlite3_bind_double(this->sql_stmt, this->id_index, _data);
}

int storage::database::part::binder::operator()(const std::string& _data) const
{
	return sqlite3_bind_text(this->sql_stmt, this->id_index, _data.data(), static_cast<int> (_data.size()), SQLITE_TRANSIENT);
}

int storage::database::part::binder::operator()(const blob& _data) const
{
	return sqlite3_bind_blob(this->sql_stmt, this->id_index, _data.data(), static_cast<int> (_data.size()), SQLITE_TRANSIENT);
}
