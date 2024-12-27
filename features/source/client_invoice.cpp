/********************************************************
 * Contents: Client invoice interface implementation
 * Author: Dawid J. Blom
 * Date: December 9, 2024
 *
 * NOTE:
 *******************************************************/
#include <client_invoice.h>
#include <iostream>

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
                unsigned int number{1};

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
        }

        return data;
}
