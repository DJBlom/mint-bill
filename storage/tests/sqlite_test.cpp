/*******************************************************************************
 * @file    sqlite_test.cpp
 * @brief   Unit tests for the SQLite/SQLCipher database wrapper and helpers.
 *
 * @details This test file verifies the behavior of the encrypted SQLite wrapper
 *          (storage::database::sqlite) and its supporting SQL operation
 *          helper (storage::database::part::sql_operations).
 *
 *          The following aspects are validated:
 *
 *            - Correct construction and error handling:
 *                • Throwing on invalid database paths or passwords.
 *                • Ensuring encrypted database access via SQLCipher.
 *
 *            - Transaction handling:
 *                • Rejecting empty transaction queries.
 *                • Successfully executing BEGIN/COMMIT sequences.
 *
 *            - Parameterized INSERT/UPDATE operations (usert):
 *                • Rejecting empty SQL strings and empty parameter lists.
 *                • Detecting mismatched parameter counts.
 *                • Handling execution failures when parameters are invalid.
 *                • Successfully inserting/updating rows with valid parameters.
 *
 *            - SELECT query behavior:
 *                • Handling empty queries and/or empty parameter lists safely.
 *                • Returning empty result sets in error/invalid-parameter cases.
 *                • Returning non-empty rows for valid queries and parameters.
 *                • Exercising column conversion for INTEGER, REAL, TEXT, and
 *                  BLOB-like data through the variant-based row representation.
 *
 *            - sql_operations helper:
 *                • Preparing SQL statements on construction and throwing on
 *                  invalid input (null connection, empty SQL, syntax errors).
 *                • Binding parameter vectors to positional placeholders and
 *                  catching mismatched counts.
 *                • Executing single-step (non-SELECT) statements.
 *                • Executing multi-step (SELECT) statements and collecting
 *                  rows into storage::database::part::rows.
 *
 *          Together, these tests provide regression coverage for the database
 *          abstraction layer, ensuring robust parameter binding, execution, and
 *          error handling around the SQLCipher-backed storage engine.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <string>
#include <iostream>
#include <sstream>
#include <variant>
#include <string>
#include <iomanip>
#include <cstddef>
#include <vector>
#include <errors.h>
#include <sqlite.h>
extern "C"
{

}

static std::vector<storage::database::param_values> good_params = {
	std::string("test"),
	std::string("odn@gmail.com"),
	std::string("123456789"),
	std::string("Geelsterd 8"),
	std::string("12345"),
	std::string("George")
};

static std::string syntax_error_sql_query{R"SQL(
	INSERT INTO admin (business_id, bank_name, branch_code, account_number, app_password, client_message)
	VALUES (?,?,?,?,?,?)
	ON CONFLICT(business_id) UPDATE SET
		bank_name=excluded.bank_name, branch_code=excluded.branch_code,
		account_number=excluded.account_number, app_password=excluded.app_password;
	)SQL"};

static std::string good_sql_query{R"SQL(
	INSERT INTO business_details (business_name, email_address, contact_number, street, area_code, town_name)
	VALUES (?,?,?,?,?,?)
	ON CONFLICT(email_address) DO UPDATE SET
		business_name=excluded.business_name, contact_number=excluded.contact_number,
		street=excluded.street, area_code=excluded.area_code, town_name=excluded.town_name;
	)SQL"};

static std::string good_sql_query_blob_and_real_data{R"SQL(
		INSERT INTO labor (labor_id, quantity, description, amount, invoice_id)
		VALUES (?,?,?,?,?)
		ON CONFLICT(labor_id) DO UPDATE SET
			quantity=excluded.quantity, description=excluded.description,
			amount=excluded.amount, invoice_id=excluded.invoice_id;
		)SQL"};



/**********************************TEST LIST************************************
 * 1) Open connection to a SQLite3 database. (Done)
 * 2) Close the connection to a SQLite3 database. (Done)
 * 3) Execute any SQL queries. (Done)
 * 4) Ensure that the database is encrypted. (Done)
 * 5) Ensure database connection can handle multiple threads. (Done)
 * 6) Ensure transaction compatibility.
 ******************************************************************************/
TEST_GROUP(sqlite_test)
{
	const std::string db_file{"../storage/tests/sql_wrapper_test.db"};
	const std::string db_password{"123456789"};
        storage::database::sqlite db{db_file, db_password};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(sqlite_test, bad_construction_throws_type)
{
	const std::string bad_db_password{"1234567890"};
	CHECK_THROWS(app::errors, storage::database::sqlite("../test.db", ""));
	CHECK_THROWS(app::errors, storage::database::sqlite("", "1234"));
	CHECK_THROWS(app::errors, storage::database::sqlite("../test.db", db_password));
	CHECK_THROWS(app::errors, storage::database::sqlite(db_file, bad_db_password));
}

TEST(sqlite_test, transaction_capability_empty_sql_query)
{
	CHECK_EQUAL(false, db.transaction(""));
}

TEST(sqlite_test, transaction_capability_begin_query)
{
	CHECK_EQUAL(true, db.transaction("BEGIN IMMEDIATE;"));
	CHECK_EQUAL(true, db.transaction("COMMIT;"));
}

TEST(sqlite_test, usert_empty_sql_query_parameter)
{
	std::string bad_sql_query{""};

	CHECK_EQUAL(false, db.usert(bad_sql_query, good_params));
}

TEST(sqlite_test, usert_empty_argument)
{
	std::vector<storage::database::param_values> bad_params;

	CHECK_EQUAL(false, db.usert(good_sql_query, bad_params));
}

TEST(sqlite_test, usert_wrong_number_of_sql_parameters)
{
	std::vector<storage::database::param_values> bad_params = {
		std::string("FedBank"),
		std::string("001"),
		std::string("123456789"),
		std::string("s3cr3t!"),
		std::string("Thanks for your business")
	};

	CHECK_EQUAL(false, db.usert(good_sql_query, bad_params));
}

TEST(sqlite_test, usert_unable_to_step_and_execute_sql_query)
{
	std::nullptr_t business_name;
	std::vector<storage::database::param_values> bad_params = {
		1LL,
		business_name,
		std::string("001"),
		std::string("123456789"),
		std::string("s3cr3t!"),
		std::string("Thanks for your business")
	};

	CHECK_EQUAL(false, db.usert(good_sql_query, bad_params));
}

TEST(sqlite_test, usert_good_parameters)
{
	CHECK_EQUAL(true, db.usert(good_sql_query, good_params));
}

TEST(sqlite_test, select_with_empty_params)
{
	std::vector<storage::database::param_values> params;
	storage::database::part::rows rows{db.select(R"SQL(
		SELECT bank_name FROM ADMIN WHERE business_id = ?;
		)SQL", params)};

	CHECK_EQUAL(true, rows.empty());
}

TEST(sqlite_test, select_empty_query)
{
	std::vector<storage::database::param_values> params = {
		1LL
	};
	storage::database::part::rows rows{db.select("", params)};

	CHECK_EQUAL(true, rows.empty());
}

TEST(sqlite_test, select_fail_to_bind_parameters)
{
	std::vector<storage::database::param_values> params = {
		1LL,
		1LL
	};
	storage::database::part::rows rows{db.select(R"SQL(
		SELECT bank_name FROM ADMIN where business_id = ?
		)SQL", params)};

	CHECK_EQUAL(true, rows.empty());
}

TEST(sqlite_test, select_empty_sql_query_statement)
{
	std::vector<storage::database::param_values> params = {
		 sqlite3_int64{1}
	};
	storage::database::part::rows rows{db.select("", params)};

	CHECK_EQUAL(true, rows.empty());
}

TEST(sqlite_test, select_no_params_and_query)
{
	storage::database::part::rows rows{db.select("")};

	CHECK_EQUAL(true, rows.empty());
}

TEST(sqlite_test, select_no_params)
{
	storage::database::part::rows rows{db.select(R"SQL(
		SELECT business_name FROM business_details;
	)SQL")};

	CHECK_EQUAL(false, rows.empty());
}

TEST(sqlite_test, select_with_good_parameters)
{
	std::vector<storage::database::param_values> params = {
		1LL
	};
	storage::database::part::rows rows{db.select(R"SQL(
		  SELECT business_name
		  FROM business_details
		  WHERE business_id = ?
		)SQL", params)};

	CHECK_EQUAL(false, rows.empty());
}

TEST(sqlite_test, select_blob_data)
{
	std::vector<storage::database::param_values> params = {
		1LL
	};
	(void)db.transaction("BEGIN IMMEDIATE;");
	storage::database::part::rows rows{db.select(R"SQL(
		SELECT quantity, description, amount
		FROM labor WHERE labor_id = ?
		)SQL", params)};
	(void)db.transaction("COMMIT;");

	CHECK_EQUAL(true, rows.empty());
}







/**********************************TEST LIST************************************
 * 1) Prepare the sql statement on creation. (Done)
 * 2) Bind parameters to the positional arguments. (Done)
 * 3) execute the sql statements. (Done)
 ******************************************************************************/
TEST_GROUP(sql_operations_test)
{
	sqlite3 *database{nullptr};
	std::string pass{"123456789"};
	void setup()
	{
		sqlite3_open_v2("../storage/tests/sql_wrapper_test.db", &database, (SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX), nullptr);
		sqlite3_key(database, pass.data(), static_cast<int> (pass.size()));
	}

	void teardown()
	{
		sqlite3_close_v2(database);
		database = nullptr;
	}
};

TEST(sql_operations_test, prepare_sql_stmt_construction)
{
	CHECK_THROWS(app::errors, storage::database::part::sql_operations(nullptr, good_sql_query));
	CHECK_THROWS(app::errors, storage::database::part::sql_operations(database, ""));
	CHECK_THROWS(app::errors, storage::database::part::sql_operations(database, syntax_error_sql_query));
}

TEST(sql_operations_test, prepare_sql_stmt_without_assigned_values)
{
	std::vector<storage::database::param_values> params;
	storage::database::part::sql_operations operations{database, good_sql_query};

	CHECK_EQUAL(false, operations.bind_params(params));
}

TEST(sql_operations_test, prepare_sql_statement_and_fail_to_execute_sql_query)
{
	storage::database::part::sql_operations operations{database, good_sql_query};

	CHECK_EQUAL(false, operations.single_execute());
}

TEST(sql_operations_test, prepare_sql_stmt_with_assigned_values)
{
	storage::database::part::sql_operations operations{database, good_sql_query};

	CHECK_EQUAL(true, operations.bind_params(good_params));
}

TEST(sql_operations_test, prepare_sql_statement_and_execute_sql_query)
{
	storage::database::part::sql_operations operations{database, good_sql_query};
	(void) operations.bind_params(good_params);

	CHECK_EQUAL(true, operations.single_execute());
}

TEST(sql_operations_test, select_good_parameter)
{
	std::vector<storage::database::param_values> params = {
		1LL
	};
	std::string sql_query{R"SQL(SELECT business_name FROM business_details WHERE business_id = ?)SQL"};
	storage::database::part::sql_operations operations{database, sql_query};
	(void) operations.bind_params(params);
	storage::database::part::rows rows{operations.multi_execute()};

	CHECK_EQUAL(false, rows.empty());
}
