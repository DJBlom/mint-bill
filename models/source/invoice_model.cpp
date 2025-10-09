/********************************************************
 * Contents: Client invoice interface implementation
 * Author: Dawid J. Blom
 * Date: December 9, 2024
 *
 * NOTE:
 *******************************************************/
#include <client_invoice.h>
#include <client_data.h>
#include <iostream>
#include <invoice_pdf.h>

feature::client_invoice::~client_invoice() {}

std::vector<std::any> feature::client_invoice::load(const std::string& _business_name) const
{
	std::vector<std::any> data;
        if (!_business_name.empty())
        {
                for (int i = 1; i <= 50; ++i)
                {
			data::pdf_invoice pdf_invoice_data{};
			data::client client_data;
			client_data.set_business_name(_business_name);
			client_data.set_business_address("Geelsterd 8");
			client_data.set_business_area_code("543543");
			client_data.set_business_town_name("George");
			client_data.set_cellphone_number("0832315944");
			client_data.set_email("dmnsstmtest@gmail.com dawidjblom@gmail.com");
			client_data.set_vat_number("3241324321413");
			client_data.set_statement_schedule("4,4");
			pdf_invoice_data.set_client(client_data);

			data::admin business_data;
			business_data.set_name("T.M Engineering");
			business_data.set_address("geelsterd 8");
			business_data.set_area_code("5432");
			business_data.set_town("george");
			business_data.set_cellphone("0832315944");
			business_data.set_email("dmnsstmtest@gmail.com");
			business_data.set_bank("Standard Bank");
			business_data.set_branch_code("043232");
			business_data.set_account_number("0932443824");
			business_data.set_client_message("Thank you for your support");
			business_data.set_password("bxwx eaku ndjj ltda");
			pdf_invoice_data.set_business(business_data);
                        const int size{50};
                        std::vector<data::column> vec{};
                        for (unsigned int j = 0; j < size; ++j)
                        {
                                std::string description{"Machining"};
                                data::column expected{};
                                expected.set_quantity(j);
                                expected.set_description(description);
                                expected.set_amount(5234567 + j + .0);
                                vec.push_back(expected);
                        }

                        std::string order_number{"order number 123"};
                        std::string card_number{"24/md"};
                        std::string paid_status{"Not Paid"};
                        std::string date{"2023-09-04"};
                        std::string description_total{"1234.00"};
                        std::string material_total{"1234.00"};
                        std::string grand_total{"2468.00"};
                        std::string number{std::to_string(i)};

                        data::invoice invoice_data;
                        invoice_data.set_business_name(_business_name);
                        invoice_data.set_invoice_number(number);
                        invoice_data.set_invoice_date(date);
                        invoice_data.set_paid_status(paid_status);
                        invoice_data.set_job_card_number(card_number);
                        invoice_data.set_order_number(order_number);
                        invoice_data.set_description_total(description_total);
                        invoice_data.set_material_total(material_total);
                        invoice_data.set_grand_total(grand_total);
                        invoice_data.set_material_column(vec);
                        invoice_data.set_description_column(vec);
			pdf_invoice_data.set_invoice(invoice_data);

			if (pdf_invoice_data.is_valid())
			{
				data.push_back(pdf_invoice_data);
			}
			else
			{
				std::cout << "PDF Invoice Data is not valid\n";
			}
                }
        }

        return data;
}

data::email feature::client_invoice::prepare_for_email(const std::vector<std::any>& _pdf_invoices) const
{
	data::email email_data;
	for (const std::any& pdf_invoice : _pdf_invoices)
	{
		data::pdf_invoice pdf_invoice_data{std::any_cast<data::pdf_invoice> (pdf_invoice)};
		email_data.set_client(pdf_invoice_data.get_client());
		email_data.set_business(pdf_invoice_data.get_business());
		break;
	}

	std::vector<std::future<std::string>> pdf_documents;
	std::transform(_pdf_invoices.cbegin(),
			_pdf_invoices.cend(),
			std::back_inserter(pdf_documents),
			[] (const std::any& _pdf_invoice) {
				return std::async(std::launch::async, [&_pdf_invoice] {
					feature::invoice_pdf pdf;
					return pdf.generate(_pdf_invoice);
				});
			});


	std::vector<std::string> pdfs;
	std::transform(
		std::make_move_iterator(pdf_documents.begin()),
		std::make_move_iterator(pdf_documents.end()),
		std::back_inserter(pdfs),
		[](std::future<std::string> _pdf) {
			return _pdf.get();
		});


	email_data.set_subject("Invoice");
	email_data.set_attachments(pdfs);
	if (email_data.is_valid())
	{
		std::cout << "Email data is valid\n";
	}

	return email_data;
}

std::vector<std::string> feature::client_invoice::prepare_for_print(const std::vector<std::any>& _pdf_invoice) const
{
	std::vector<std::future<std::string>> pdf_documents;
	std::transform(_pdf_invoice.cbegin(),
			_pdf_invoice.cend(),
			std::back_inserter(pdf_documents),
			[] (const std::any& _pdf_invoice) {
				return std::async(std::launch::async, [&_pdf_invoice] {
					feature::invoice_pdf pdf;
					return pdf.generate(_pdf_invoice);
				});
			});


	std::vector<std::string> pdfs;
	std::transform(
		std::make_move_iterator(pdf_documents.begin()),
		std::make_move_iterator(pdf_documents.end()),
		std::back_inserter(pdfs),
		[](std::future<std::string> _pdf) {
			return _pdf.get();
		});

	return pdfs;
}

bool feature::client_invoice::save(const data::invoice& data)
{
        bool updated{false};
        if (data.is_valid())
        {
                std::cout << "admin name: " << data.get_business_name() << std::endl;
                std::cout << "Invoice number: " << data.get_invoice_number() << std::endl;
                std::cout << "Invoice date: " << data.get_invoice_date() << std::endl;
                std::cout << "Job card number: " << data.get_job_card_number() << std::endl;
                std::cout << "Order number: " << data.get_order_number() << std::endl;


                std::cout << "Description!!!!!!!!!!!!!!!!\n";
                std::vector<data::column> description_data = data.get_description_column();
                for (const auto& column : description_data)
                {
                        std::cout << "Quantity: " << column.get_quantity() << std::endl;
                        std::cout << "Description: " << column.get_description() << std::endl;
                        std::cout << "Amount: " << column.get_amount() << std::endl;
                }

                std::cout << "Material!!!!!!!!!!!!!!!!!!!\n";
                std::vector<data::column> material_data = data.get_material_column();
                for (const auto& column : material_data)
                {
                        std::cout << "Quantity: " << column.get_quantity() << std::endl;
                        std::cout << "Description: " << column.get_description() << std::endl;
                        std::cout << "Amount: " << column.get_amount() << std::endl;
                }

                std::cout << "Description Total: " << data.get_description_total() << std::endl;
                std::cout << "Material Total: " << data.get_material_total() << std::endl;
                std::cout << "Grand Total: " << data.get_grand_total() << std::endl;

                updated = true;
        }

        return updated;
}

std::vector<data::invoice> feature::client_invoice::search(const std::string& business_name)
{
        std::vector<data::invoice> data{};
        if (!business_name.empty())
        {
                for (int i = 1; i <= 50; ++i)
                {
                        const int size{50};
                        data::invoice _invoice;
                        std::vector<data::column> vec{};
                        for (unsigned int j = 0; j < size; ++j)
                        {
                                std::string description{"Machining"};
                                data::column expected{};
                                expected.set_quantity(j);
                                expected.set_description(description);
                                expected.set_amount(5234567 + j + .0);
                                vec.push_back(expected);
                        }

                        std::string order_number{"order number 123"};
                        std::string card_number{"24/md"};
                        std::string paid_status{"Not Paid"};
                        std::string date{"2023-09-04"};
                        std::string description_total{"1234.00"};
                        std::string material_total{"1234.00"};
                        std::string grand_total{"2468.00"};
                        std::string number{std::to_string(i)};

                        _invoice.set_business_name(business_name);
                        _invoice.set_invoice_number(number);
                        _invoice.set_invoice_date(date);
                        _invoice.set_paid_status(paid_status);
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
