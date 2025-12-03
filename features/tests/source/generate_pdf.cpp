/********************************************************
 * Contents: Generate dummy pdf implementation
 * Author: Dawid J. Blom
 * Date: December 30, 2024
 *
 * NOTE:
 *******************************************************/
#include <generate_pdf.h>
#include <string>
#include <ostream>
#include <client_data.h>
#include <business_data.h>
#include <invoice_data.h>
#include <pdf_invoice_data.h>
#include <pdf_statement_data.h>


bool test::generate_invoice_pdf(const std::string& _pdf_data, const std::string& _suffix)
{
        bool generated{false};
        if (!_pdf_data.empty())
        {
                std::ofstream file("invoice_pdf_test" + _suffix + ".pdf", std::ios::binary);
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

bool test::generate_statement_pdf(const std::string& _pdf_data, const std::string& _suffix)
{
        bool generated{false};
        if (!_pdf_data.empty())
        {
                std::ofstream file("statement_pdf_test" + _suffix + ".pdf", std::ios::binary);
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

data::admin test::generate_business_data()
{
        data::admin expected;
        expected.set_name("T.M. Engineering");
        expected.set_address("Geelsterd 8");
        expected.set_area_code("5432");
        expected.set_town("George");
        expected.set_cellphone("0832315944");
        expected.set_email("dmnsstmtest@gmail.com");
        expected.set_bank("Standard Bank");
        expected.set_branch_code("043232");
        expected.set_account_number("0932443824");
        expected.set_client_message("Thank you for your support");
        expected.set_password("bxwx eaku ndjj ltda");

        return expected;
}

data::client test::generate_client_data()
{
        data::client expected;
        expected.set_name("Client admin");
        expected.set_address("Geelsterd 8");
        expected.set_area_code("543543");
        expected.set_town("George");
        expected.set_cellphone("0832315944");
        expected.set_email("dmnsstmtest@gmail.com");
        expected.set_vat_number("3241324321413");
        expected.set_statement_schedule("4,4");

        return expected;
}

data::client test::generate_client_data_multiple_emails()
{
        data::client expected;
        expected.set_name("Client admin");
        expected.set_address("Geelsterd 8");
        expected.set_area_code("543543");
        expected.set_town("George");
        expected.set_cellphone("0832315944");
        expected.set_email("dmnsstmtest@gmail.com dawidjblom@gmail.com");
        expected.set_vat_number("3241324321413");
        expected.set_statement_schedule("4,4");

        return expected;
}

data::invoice test::generate_invoice_data(const std::string& _desc)
{
        const int size{50};
        std::vector<data::column> vec{};
        std::string description{_desc};
        for (unsigned int i = 0; i < size; ++i)
        {
                data::column expected{};
                expected.set_quantity(i);
                expected.set_description(description);
                expected.set_amount(5545675 + i + .0);
                expected.set_row_number(i);
		if (i % 2 == 0)
		{
			expected.set_is_description(1);
		}
		else
		{
			expected.set_is_description(0);
		}
		if (expected.is_valid())
		{
			vec.push_back(expected);
		}
        }
        std::string order_number{"order number 123"};
        std::string card_number{"24/md"};
        std::string date{"2023-09-04"};
        std::string paid_status{"Not Paid"};
        std::string name{"Client admin"};
        std::string description_total{"1234.00"};
        std::string material_total{"1234.00"};
        std::string number{"2"};

        data::invoice expected;
        expected.set_name(name);
        expected.set_id(number);
        expected.set_date(date);
        expected.set_paid_status(paid_status);
        expected.set_job_card_number(card_number);
        expected.set_order_number(order_number);
        expected.set_description_total(description_total);
        expected.set_material_total(material_total);
	std::ostringstream goss{""};
	goss << std::fixed << std::setprecision(2) << vec[0].get_amount();
        expected.set_grand_total(goss.str());
        expected.set_material_column(vec);
        expected.set_description_column(vec);

        return expected;
}

data::invoice test::generate_invoice_data(const std::string& _desc, const int& num)
{
        const int size{50};
        std::vector<data::column> vec{};
        for (unsigned int i = 0; i < size; ++i)
        {
                data::column expected{};
                expected.set_quantity(i);
                expected.set_description(_desc);
                expected.set_amount(5545675 + i + .0);
                expected.set_row_number(static_cast<long long>(i));
		if (i % 2 == 0)
		{
			expected.set_is_description(1);
		}
		else
		{
			expected.set_is_description(0);
		}
		vec.push_back(expected);
        }
        std::string order_number{"order number 123"};
        std::string card_number{"24/md"};
        std::string date{"2023-09-04"};
        std::string name{"Test admin"};
        std::string description_total{"1234.00"};
        std::string material_total{"1234.00"};
        std::string grand_total{"1234.00"};

        data::invoice expected;
        expected.set_name(name);
        expected.set_id(std::to_string(num));
        expected.set_date(date);
        expected.set_job_card_number(card_number);
        expected.set_order_number(order_number);
        expected.set_description_total(description_total);
        expected.set_material_total(material_total);
        expected.set_grand_total(std::to_string(vec[0].get_amount()));
        expected.set_material_column(vec);
        expected.set_description_column(vec);

        return expected;
}

data::statement test::generate_statement_data()
{
	data::statement expected{};
	expected.set_id("1");
	expected.set_name("Client admin");
	expected.set_date("Dec-1-2025");
	expected.set_paid_status("Not paid");
	expected.set_period_start("Dec-1-2025");
	expected.set_period_end("Dec-11-2025");

        return expected;
}

std::vector<std::any> test::get_pdf_statements(const std::string& _business_name)
{
	std::vector<std::any> temp{};
	if (_business_name.empty())
	{

	}
	else
	{
		for (int i = 0; i < 100; ++i)
		{
			data::pdf_invoice pdf_invoice_data{};
			data::admin admin_data{generate_business_data()};
			pdf_invoice_data.set_business(admin_data);

			data::client client_data{generate_client_data()};
			pdf_invoice_data.set_client(client_data);

			std::vector<data::pdf_invoice> pdf_invoices{};
			for (int j = 0; j < 50; ++j)
			{
				data::invoice invoice_data{generate_invoice_data("Test description")};
				pdf_invoice_data.set_invoice(invoice_data);
				pdf_invoices.push_back(pdf_invoice_data);
			}
			data::pdf_statement pdf_statement_data{};
			pdf_statement_data.set_number(std::to_string(i));
			pdf_statement_data.set_date("02/24/2025");
			pdf_statement_data.set_total("2056.00");
			pdf_statement_data.set_statement(generate_statement_data());
			pdf_statement_data.set_pdf_invoices(pdf_invoices);

			temp.push_back(pdf_statement_data);
		}
	}

	return temp;
}


std::vector<std::any> test::get_pdf_invoices(const std::string& _business_name)
{
	std::vector<std::any> data;
        if (!_business_name.empty())
        {
                for (int i = 1; i <= 50; ++i)
                {
			std::vector<data::pdf_invoice> pdf_invoices{};
			data::pdf_invoice pdf_invoice_data{};
			data::client client_data{generate_client_data()};
			pdf_invoice_data.set_client(client_data);

			data::admin admin_data{generate_business_data()};
			pdf_invoice_data.set_business(admin_data);

			data::invoice invoice_data{generate_invoice_data("Test description")};
			pdf_invoice_data.set_invoice(invoice_data);

			if (pdf_invoice_data.is_valid())
			{
				data.push_back(pdf_invoice_data);
			}
                }
        }

        return data;
}
