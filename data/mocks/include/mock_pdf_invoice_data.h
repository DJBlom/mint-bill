/*****************************************************************************
 * @file mock_pdf_invoice_data.h
 *
 * @brief
 *   Helper factory functions for constructing test data objects used in
 *   PDF invoice and statement unit tests.
 *
 * @details
 *   This header declares a small set of free functions that generate
 *   pre-populated data model instances for use in the PDF-related test
 *   suites:
 *
 *     - retrieve_client_data() / retrieve_bad_client_data()
 *         Return valid and intentionally invalid data::client objects.
 *
 *     - retrieve_invoice_data() / retrieve_bad_invoice_data()
 *         Return valid and intentionally invalid data::invoice objects.
 *
 *     - retrieve_business_data() / retrieve_bad_business_data()
 *         Return valid and intentionally invalid data::admin objects
 *         representing the business issuing the invoice.
 *
 *   Centralizing these factories keeps the tests focused on the behavior
 *   of pdf_invoice / pdf_statement and avoids repetitive boilerplate
 *   setup. It also makes it easy to adjust test datasets in a single
 *   place if the underlying data model constraints change.
 *****************************************************************************/
#ifndef _MOCK_PDF_INVOICE_DATA_H_
#define _MOCK_PDF_INVOICE_DATA_H_
#include <client_data.h>
#include <invoice_data.h>
#include <admin_data.h>

data::client retrieve_client_data();
data::client retrieve_bad_client_data();
data::invoice retrieve_invoice_data();
data::invoice retrieve_bad_invoice_data();
data::admin retrieve_business_data();
data::admin retrieve_bad_business_data();
#endif
