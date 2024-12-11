/********************************************************
 * Contents: Client invoice interface implementation
 * Author: Dawid J. Blom
 * Date: December 9, 2024
 *
 * NOTE:
 *******************************************************/
#include <client_invoice.h>
#include <iostream>

data::invoice invoice_data{};

feature::invoice::~invoice() {}

data::invoice feature::invoice::load(const std::string& business_name, const interface::storage& db)
{
        data::invoice data{};
        if (!business_name.empty() && db.update())
        {
        }

        return data;
}

bool feature::invoice::save(const data::invoice& data, const interface::storage& db)
{
        bool updated{false};
        if (data.is_valid())
        {
                //invoice_data = data;
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

                invoice_data = std::move(data);
                updated = db.update();
        }

        return updated;
}

std::vector<data::invoice> feature::invoice::search(const std::string& business_name, const interface::storage& db)
{
        std::vector<data::invoice> data{};
        if (!business_name.empty() && db.update())
        {
        }

        return data;
}
