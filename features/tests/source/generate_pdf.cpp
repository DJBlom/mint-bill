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
