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
#include <fstream>
#include <client_data.h>
#include <invoice_data.h>
#include <business_data.h>
#include <pdf_invoice_data.h>
extern "C"
{

}

static bool generate_invoice_pdf(const std::string& _pdf_data, const std::string& _suffix);
static data::business generate_business_data();
static data::client generate_client_data();
static data::invoice generate_invoice_data();

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
        data::pdf_invoice pdf_data;
        data::business business_data{generate_business_data()};
        pdf_data.set_business(business_data);
        data::client client_data{generate_client_data()};
        pdf_data.set_client(client_data);
        data::invoice invoice_data{generate_invoice_data()};
        pdf_data.set_invoice(invoice_data);
        std::string pdf_file_data{pdf.generate(pdf_data)};

        bool result{generate_invoice_pdf(pdf_file_data, "1")};

        CHECK_EQUAL(true, result);
}

static bool generate_invoice_pdf(const std::string& _pdf_data, const std::string& _suffix)
{
        bool generated{false};
        if (!_pdf_data.empty())
        {
                std::ofstream file("test" + _suffix + ".pdf", std::ios::binary);
                if (!file.is_open()) {
                        return generated;
                }

                file.write(_pdf_data.data(), static_cast<std::streamsize> (_pdf_data.size()));
                if (file.fail()) {
                        file.close();
                        return generated;
                }

                file.close();
                generated = true;
        }

        return generated;
}

static data::business generate_business_data()
{
        data::business expected;
        expected.set_name("T.M. Engineering");
        expected.set_address("Geelsterd 8");
        expected.set_area_code("5432");
        expected.set_town("George");
        expected.set_cellphone("0832315944");
        expected.set_email("odn@gmail.com");
        expected.set_bank("Standard Bank");
        expected.set_branch_code("043232");
        expected.set_account_number("0932443824");
        expected.set_client_message("Thank you for your support");
        expected.set_password("fdasfdsafdsf");

        return expected;
}

static data::client generate_client_data()
{
        data::client expected;
        expected.set_business_name("Client Business");
        expected.set_business_address("Geelsterd 8");
        expected.set_business_area_code("543543");
        expected.set_business_town_name("George");
        expected.set_cellphone_number("0832315944");
        expected.set_email("odn@gmail.com");
        expected.set_vat_number("3241324321413");
        expected.set_statement_schedule("4,4");

        return expected;
}

static data::invoice generate_invoice_data()
{
        const int size{5};
        std::vector<data::column> vec{};
        //std::string long_description{"Machining steel"};
        std::string long_description{"Beneath the sky so vast and blue, Where whispers weave the morning dew, The trees stand tall, their shadows play, A dance of life, a soft ballet. The river hums a timeless tune, Reflecting light from sun to moon, It carries dreams to oceans deep, A secret world where hopes may sleep. The stars ignite the velvet night, Their spark, a beacon’s gentle light. Through every breath, through joy and strife, We live, we dream—the dance of life. Forever flows this fleeting glow. Test completed, good!"};
        for (unsigned int i = 0; i < size; ++i)
        {
                data::column expected{};
                expected.set_quantity(i);
                expected.set_description(long_description);
                expected.set_amount(5545675 + i + .0);
                vec.push_back(expected);
        }
        std::string order_number{"order number 123"};
        std::string card_number{"24/md"};
        std::string date{"2023-09-04"};
        std::string name{"Test Business"};
        std::string description_total{"1234.00"};
        std::string material_total{"1234.00"};
        std::string grand_total{"1234.00"};
        unsigned int number{1};

        data::invoice expected;
        expected.set_business_name(name);
        expected.set_invoice_number(number);
        expected.set_invoice_date(date);
        expected.set_job_card_number(card_number);
        expected.set_order_number(order_number);
        expected.set_description_total(description_total);
        expected.set_material_total(material_total);
        expected.set_grand_total(grand_total);
        expected.set_material_column(vec);
        expected.set_description_column(vec);

        return expected;
}
