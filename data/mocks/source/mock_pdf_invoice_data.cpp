#include <mock_pdf_invoice_data.h>


data::client retrieve_client_data()
{
        data::client expected;
        expected.set_business_name("TME");
        expected.set_business_address("Geelsterd 8");
        expected.set_business_area_code("543543");
        expected.set_business_town_name("George");
        expected.set_cellphone_number("0832315944");
        expected.set_email("odn@gmail.com");
        expected.set_vat_number("3241324321413");
        expected.set_statement_schedule("4,4");

        return expected;
}

data::client retrieve_bad_client_data()
{
        data::client expected;
        expected.set_business_name("");
        expected.set_business_address("Geelsterd 8");
        expected.set_business_area_code("543543");
        expected.set_business_town_name("George");
        expected.set_cellphone_number("0832315944");
        expected.set_email("odn@gmail.com");
        expected.set_vat_number("3241324321413");
        expected.set_statement_schedule("4,4");

        return expected;
}

data::invoice retrieve_invoice_data()
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
        data::invoice expected;
        expected.set_business_name("Test Business");
        expected.set_invoice_number("134");
        expected.set_invoice_date("2023-09-04");
        expected.set_paid_status("Not Paid");
        expected.set_job_card_number("24/md");
        expected.set_order_number("order 123");
        expected.set_description_total("1234.00");
        expected.set_material_total("1234.00");
        expected.set_grand_total("1234.00");
        expected.set_material_column(vec);
        expected.set_description_column(vec);

        return expected;
}

data::invoice retrieve_bad_invoice_data()
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
        data::invoice expected;
        expected.set_business_name("");
        expected.set_invoice_number("134");
        expected.set_invoice_date("2023-09-04");
        expected.set_paid_status("Not Paid");
        expected.set_job_card_number("24/md");
        expected.set_order_number("order 123");
        expected.set_description_total("1234.00");
        expected.set_material_total("1234.00");
        expected.set_grand_total("1234.00");
        expected.set_material_column(vec);
        expected.set_description_column(vec);

        return expected;
}

data::business retrieve_business_data()
{
        data::business expected;
        expected.set_name("tme");
        expected.set_address("geelsterd 8");
        expected.set_area_code("5432");
        expected.set_town("george");
        expected.set_cellphone("0832315944");
        expected.set_email("odn@gmail.com");
        expected.set_bank("Standard Bank");
        expected.set_branch_code("043232");
        expected.set_account_number("0932443824");
        expected.set_client_message("Thank you for your support");
        expected.set_password("");

        return expected;
}

data::business retrieve_bad_business_data()
{
        data::business expected;
        expected.set_name("");
        expected.set_address("geelsterd 8");
        expected.set_area_code("5432");
        expected.set_town("george");
        expected.set_cellphone("0832315944");
        expected.set_email("odn@gmail.com");
        expected.set_bank("Standard Bank");
        expected.set_branch_code("043232");
        expected.set_account_number("0932443824");
        expected.set_client_message("Thank you for your support");
        expected.set_password("");

        return expected;
}
