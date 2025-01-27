/********************************************************
 * Contents: Client invoice interface implementation
 * Author: Dawid J. Blom
 * Date: December 9, 2024
 *
 * NOTE:
 *******************************************************/
#include <client_invoice.h>
#include <pdf_invoice_data.h>
#include <client_data.h>
#include <iostream>
#include <pdf.h>

feature::invoice::~invoice() {}

data::invoice feature::invoice::load(const std::string& business_name, const interface::storage& db)
{
        data::invoice data{};
        if (!business_name.empty() && db.is_open())
        {
                const int size{50};
                std::vector<data::column> vec{};
                for (unsigned int i = 0; i < size; ++i)
                {
                        data::column expected{};
                        expected.set_quantity(i);
                        expected.set_description("machining");
                        expected.set_amount(55 + i + .0);
                        vec.push_back(expected);
                }
                std::string order_number{"order number 123"};
                std::string card_number{"24/md"};
                std::string date{"2023-09-04"};
                std::string name{"Test Business"};
                std::string description_total{"1234.00"};
                std::string material_total{"1234.00"};
                std::string grand_total{"1234.00"};
                std::string number{"1"};

                data.set_business_name(name);
                data.set_invoice_number(number);
                data.set_invoice_date(date);
                data.set_job_card_number(card_number);
                data.set_order_number(order_number);
                data.set_description_total(description_total);
                data.set_material_total(material_total);
                data.set_grand_total(grand_total);
                data.set_material_column(vec);
                data.set_description_column(vec);
        }

        return data;
}

bool feature::invoice::save(const data::invoice& data, const interface::storage& db)
{
        bool updated{false};
        if (data.is_valid() && db.is_open())
        {
                std::cout << "Business name: " << data.get_business_name() << std::endl;
                std::cout << "Invoice number: " << data.get_invoice_number() << std::endl;
                std::cout << "Invoice date: " << data.get_invoice_date() << std::endl;
                std::cout << "Job card number: " << data.get_job_card_number() << std::endl;
                std::cout << "Order number: " << data.get_order_number() << std::endl;


                std::vector<data::column> description_data = data.get_description_column();
                for (const auto& column : description_data)
                {
                        std::cout << "Quantity: " << column.get_quantity() << std::endl;
                        std::cout << "Description: " << column.get_description() << std::endl;
                        std::cout << "Amount: " << column.get_amount() << std::endl;
                }

                std::vector<data::column> material_data = data.get_material_column();
                for (const auto& column : material_data)
                {
                        std::cout << "Quantity: " << column.get_quantity() << std::endl;
                        std::cout << "Description: " << column.get_description() << std::endl;
                        std::cout << "Amount: " << column.get_amount() << std::endl;
                }

                updated = true;
        }

        return updated;
}

std::vector<data::invoice> feature::invoice::search(const std::string& business_name, const interface::storage& db)
{
        std::vector<data::invoice> data{};
        if (!business_name.empty() && db.is_open())
        {
                for (int i = 1; i <= 50; ++i)
                {
                        data::invoice _invoice;
                        const int size{10};
                        std::vector<data::column> vec{};
                        for (unsigned int j = 0; j < size; ++j)
                        {
                                data::column expected{};
                                expected.set_quantity(j);
                                expected.set_description("machining");
                                expected.set_amount(55 + j + .0);
                                vec.push_back(expected);
                        }

                        std::string order_number{"order number 123"};
                        std::string card_number{"24/md"};
                        std::string date{"2023-09-04"};
                        std::string name{business_name};
                        std::string description_total{"1234.00"};
                        std::string material_total{"1234.00"};
                        std::string grand_total{"1234.00"};
                        std::string number{std::to_string(i)};

                        _invoice.set_business_name(name);
                        _invoice.set_invoice_number(number);
                        _invoice.set_invoice_date(date);
                        _invoice.set_job_card_number(card_number);
                        _invoice.set_order_number(order_number);
                        _invoice.set_description_total(description_total);
                        _invoice.set_material_total(material_total);
                        _invoice.set_grand_total(grand_total);
                        _invoice.set_material_column(vec);
                        _invoice.set_description_column(vec);

                        data.push_back(_invoice);
                }
        }

        return data;
}

bool feature::invoice::send_email(const data::invoice& _data)
{
        std::future<bool> sent{};
        if (_data.is_valid())
        {
                data::pdf_invoice pdf_invoice_data;
                data::client client_data;
                client_data.set_business_name(_data.get_business_name());
                client_data.set_business_address("Geelsterd 8");
                client_data.set_business_area_code("543543");
                client_data.set_business_town_name("George");
                client_data.set_cellphone_number("0832315944");
                client_data.set_email("dmnsstmtest@gmail.com dawidjblom@gmail.com");
                client_data.set_vat_number("3241324321413");
                client_data.set_statement_schedule("4,4");
                pdf_invoice_data.set_client(client_data);

                pdf_invoice_data.set_invoice(_data);

                data::business business_data;
                business_data.set_name("T.M Engineering");
                business_data.set_address("Geelsterd 8");
                business_data.set_area_code("6625");
                business_data.set_town("George");
                business_data.set_cellphone("0832315944");
                business_data.set_email("dmnsstmtest@gmail.com");
                business_data.set_bank("Standard Bank");
                business_data.set_branch_code("043232");
                business_data.set_account_number("0932443824");
                business_data.set_client_message("Thank you for your support!");
                business_data.set_password("bxwx eaku ndjj ltda");
                pdf_invoice_data.set_business(business_data);

                feature::pdf pdf{};
                std::string pdf_data{pdf.generate_for_email(pdf_invoice_data)};

                data::email email_data{};
                email_data.set_pdf(pdf_data);
                email_data.set_client(client_data);
                email_data.set_business(business_data);
                email_data.set_subject("Invoice");
                if (email_data.is_valid() == false)
                        std::cout << "Email data is not valid\n";
                else
                        sent = sending(email_data);

        }

        return sent.get();
}

std::future<bool> feature::invoice::sending(const data::email& _data)
{
       std::future<bool> sent{};
        if (_data.is_valid())
        {
                sent = std::move(std::async(std::launch::async, [this, _data]() {
                        return this->email.send(_data);
                }));
        }

        return sent;
}
