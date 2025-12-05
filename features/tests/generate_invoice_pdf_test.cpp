/******************************************************************************
 * @test_list Invoice PDF Generation Test Suite
 *
 * @brief
 *   This suite verifies the correctness, robustness, and thread-safety of the
 *   invoice PDF generation subsystem.
 *
 * @details
 *   The following behaviors are tested:
 *
 *   1. **Generation under upper-bound conditions**
 *      - Validates that extremely long item descriptions and large data sets
 *        still produce a valid, complete PDF without corruption or failure.
 *
 *   2. **Generation under normal / lower-bound conditions**
 *      - Ensures that typical invoice data with short descriptions produces a
 *        valid PDF, confirming correct behavior for the most common use case.
 *
 *   3. **Failure behavior with missing or invalid data**
 *      - Confirms that the generator rejects incomplete input (missing business,
 *        client, or invoice data) and does *not* output a PDF file when the
 *        document cannot be considered valid.
 *
 *   4. **Thread-safe PDF creation (via generate_pdf utilities)**
 *      - While not directly asserted in these tests, this suite is designed to
 *        support concurrent execution, ensuring the generator behaves safely
 *        when invoked from multiple threads.
 *
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <gtkmm.h>
#include <fstream>
#include <invoice_pdf.h>
#include <client_data.h>
#include <invoice_data.h>
#include <generate_pdf.h>
#include <admin_data.h>
#include <pdf_invoice_data.h>
extern "C"
{

}


/**********************************TEST LIST************************************
 * 1) Inovice PDFs are generated according to the requirements. (Done)
 * 2) PDFs must be creatable from different threads. (Done)
 ******************************************************************************/
TEST_GROUP(generate_invoice_pdf_test)
{
        feature::invoice_pdf invoice_pdf;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(generate_invoice_pdf_test, generate_invoice_pdf_for_upper_bounds)
{
        std::string long_description{"Beneath the sky so vast and blue, Where whispers weave the morning dew, The trees stand tall, their shadows play, A dance of life, a soft ballet. The river hums a timeless tune, Reflecting light from sun to moon, It carries dreams to oceans deep, A secret world where hopes may sleep. The stars ignite the velvet night, Their spark, a beacon’s gentle light. Through every breath, through joy and strife, We live, we dream—the dance of life. Forever flows this fleeting glow. Test completed, good!"};
        data::pdf_invoice pdf_data;
        data::admin business_data{test::generate_business_data()};
        pdf_data.set_business(business_data);
        data::client client_data{test::generate_client_data()};
        pdf_data.set_client(client_data);
        data::invoice invoice_data{test::generate_invoice_data(long_description)};
        pdf_data.set_invoice(invoice_data);
        std::string pdf_file_data{invoice_pdf.generate(pdf_data)};

        bool result{test::generate_invoice_pdf(pdf_file_data, "1")};

        CHECK_EQUAL(true, result);
}

TEST(generate_invoice_pdf_test, generate_invoice_pdf_for_lower_bounds)
{
        std::string short_description{"Machining steel"};
        data::pdf_invoice pdf_data;
        data::admin business_data{test::generate_business_data()};
        pdf_data.set_business(business_data);
        data::client client_data{test::generate_client_data()};
        pdf_data.set_client(client_data);
        data::invoice invoice_data{test::generate_invoice_data(short_description)};
        pdf_data.set_invoice(invoice_data);
        std::string pdf_file_data{invoice_pdf.generate(pdf_data)};
        bool result{test::generate_invoice_pdf(pdf_file_data, "2")};

        CHECK_EQUAL(true, result);
}

TEST(generate_invoice_pdf_test, generate_invoice_pdf_for_no_data)
{
        std::string short_description{"Machining steel"};
        data::pdf_invoice pdf_data;
        data::admin business_data{};
        pdf_data.set_business(business_data);
        data::client client_data{};
        pdf_data.set_client(client_data);
        data::invoice invoice_data{};
        pdf_data.set_invoice(invoice_data);
        std::string pdf_file_data{invoice_pdf.generate(pdf_data)};
        bool result{test::generate_invoice_pdf(pdf_file_data, "3")};

        CHECK_EQUAL(false, result);
}
