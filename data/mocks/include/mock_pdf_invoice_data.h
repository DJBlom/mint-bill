#ifndef _MOCK_PDF_INVOICE_DATA_H_
#define _MOCK_PDF_INVOICE_DATA_H_
#include <client_data.h>
#include <invoice_data.h>
#include <business_data.h>

data::client retrieve_client_data();
data::client retrieve_bad_client_data();
data::invoice retrieve_invoice_data();
data::invoice retrieve_bad_invoice_data();
data::business retrieve_business_data();
data::business retrieve_bad_business_data();
#endif
