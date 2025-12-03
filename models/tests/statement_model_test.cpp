/*******************************************************************************
 * Contents: Statement feature unit tests
 * Author: Dawid Blom
 * Date: June 11, 2025
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <vector>
#include <iostream>
#include <sqlite.h>
#include <pdf_statement_data.h>
#include <statement_model.h>
#include <client_model.h>
#include <invoice_model.h>
#include <client_data.h>
#include <statement_data.h>
#include <generate_pdf.h>

extern "C"
{

}


/**********************************TEST LIST************************************
 * 1)
 ******************************************************************************/
TEST_GROUP(statement_model_test)
{
	const std::string db_file{"../storage/tests/model_test.db"};
	const std::string db_password{"123456789"};
	storage::database::sqlite database{db_file, db_password};
        data::client client_data{test::generate_client_data_multiple_emails()};
	model::statement statement{db_file, db_password};
	void setup()
	{
		model::client client_model{db_file, db_password};
		(void) client_model.save(client_data);
	}

	void teardown()
	{
	}
};

TEST(statement_model_test, save_data_to_db)
{
	std::vector<data::pdf_statement> result{};
	data::statement statement_data{test::generate_statement_data()};

	CHECK_EQUAL(true, statement.save(statement_data));
}

TEST(statement_model_test, load_data_from_db)
{
	data::invoice invoice_data{test::generate_invoice_data("model testing")};
	data::statement statement_data{test::generate_statement_data()};
	model::invoice invoice_model{db_file, db_password};
	(void) invoice_model.save(invoice_data);
	(void) statement.save(statement_data);
	std::vector<data::pdf_statement> result{};
	for (const std::any& data : statement.load(invoice_data.get_name()))
	{
		data::pdf_statement pdf_statement{std::any_cast<data::pdf_statement>(data)};
		result.emplace_back(pdf_statement);
	}

	CHECK_EQUAL(false, result.empty());
}
