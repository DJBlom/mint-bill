/******************************************************************************
 * @file generate_pdf.h
 * @brief Test utility declarations for generating PDFs and test data objects.
 *
 * @details
 * This header declares a collection of helper functions used by the unit and
 * integration tests to:
 *  - Generate binary PDF files for invoices and statements from in-memory data.
 *  - Construct consistent, valid instances of admin, client, invoice, and
 *    statement data models.
 *  - Produce batches of std::any-wrapped PDF data objects for performance and
 *    scalability testing.
 *
 * Primary Responsibilities:
 *  - Provide small, focused factory functions for creating realistic test data
 *    without duplicating setup logic across tests.
 *  - Offer helpers that write raw PDF byte streams to disk for visual/manual
 *    verification or debugging.
 *  - Supply collections of pdf_invoice and pdf_statement wrappers to exercise
 *    list rendering, pagination, and bulk-processing paths.
 *
 * Key Functions:
 *  - bool generate_invoice_pdf(const std::string& pdf_bytes,
 *                              const std::string& suffix)
 *      Writes the provided PDF byte stream to a file named
 *      "invoice_pdf_test<suffix>.pdf". Returns true on success and false if
 *      the data is empty or file I/O fails.
 *
 *  - bool generate_statement_pdf(const std::string& pdf_bytes,
 *                                const std::string& suffix)
 *      Writes the provided PDF byte stream to a file named
 *      "statement_pdf_test<suffix>.pdf". Returns true on success and false if
 *      the data is empty or file I/O fails.
 *
 *  - data::admin generate_business_data()
 *      Builds a fully-populated, valid admin/business object suitable for PDF
 *      generation and e-mail tests.
 *
 *  - data::client generate_client_data()
 *      Creates a client object with a single e-mail address and valid contact
 *      information for use in invoice/statement tests.
 *
 *  - data::client generate_client_data_multiple_emails()
 *      Returns a client object intended to represent a multi-recipient e-mail
 *      scenario (e.g., comma-separated addresses), allowing tests to validate
 *      parsing and distribution logic.
 *
 *  - data::invoice generate_invoice_data(const std::string& description)
 *      Generates a realistic invoice that contains multiple line items with
 *      alternating description/material flags, calculated row numbers, and
 *      pre-filled totals. The supplied description is used for all items.
 *
 *  - data::invoice generate_invoice_data(const std::string& description,
 *                                        const int& id_number)
 *      Overload that additionally sets the invoice ID to the provided numeric
 *      value, allowing tests to create sequences of incrementing invoices.
 *
 *  - data::statement generate_statement_data()
 *      Produces a statement object with a fixed date range and status, suitable
 *      for driving statement PDF rendering.
 *
 *  - std::vector<std::any> get_pdf_statements(const std::string& business_name)
 *      Returns a collection of std::any-encoded data::pdf_statement objects
 *      for the provided business. Used to exercise bulk statement rendering
 *      paths and stress-test performance.
 *
 *  - std::vector<std::any> get_pdf_invoices(const std::string& business_name)
 *      Produces a vector of std::any-encoded data::pdf_invoice objects for the
 *      requested business, allowing tests to validate invoice PDF generation
 *      at scale.
 *
 * Usage Notes:
 *  - These helpers are defined under the test namespace and are not intended
 *    for production use.
 *  - All returned data objects aim to be valid according to their is_valid()
 *    implementations unless explicitly designed as edge cases.
 *
 ******************************************************************************/
#ifndef _TEST_GENERATE_PDF_H_
#define _TEST_GENERATE_PDF_H_
#include <fstream>
#include <any>
#include <vector>
#include <client_data.h>
#include <invoice_data.h>
#include <admin_data.h>
#include <statement_data.h>

namespace test {
        bool generate_invoice_pdf(const std::string&, const std::string&);
        bool generate_statement_pdf(const std::string&, const std::string&);
        data::admin generate_business_data();
        data::client generate_client_data();
        data::client generate_client_data_multiple_emails();
        data::invoice generate_invoice_data(const std::string&);
        data::invoice generate_invoice_data(const std::string&, const int&);
	data::statement generate_statement_data();
	std::vector<std::any> get_pdf_statements(const std::string& _business_name);
	std::vector<std::any> get_pdf_invoices(const std::string& _business_name);
}
#endif
