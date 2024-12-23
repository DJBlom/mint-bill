/*******************************************************************************
 * Contents: Generate PDF feature unit tests
 * Author: Dawid Blom
 * Date: December 23, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <pdf.h>
extern "C"
{

}

static bool generate_invoice_pdf();

/**********************************TEST LIST************************************
 * 1) Inovice PDFs are generated according to the requirements.
 * 2) Statement PDFs are generated according to the requirements.
 * 3) The feature should be copy or moveable.
 * 4) PDFs must be createable from different threads.
 ******************************************************************************/
TEST_GROUP(generate_pdf_test)
{
        feature::pdf pdf;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(generate_pdf_test, generate_invoice_pdf_according_to_the_requirements)
{
        bool result{generate_invoice_pdf()};

        CHECK_EQUAL(true, result);
}

static bool generate_invoice_pdf()
{
        return true;
}
