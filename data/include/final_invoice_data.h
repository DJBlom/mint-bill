/********************************************************
 * Contents: Final invoice data definition
 * Author: Dawid J. Blom
 * Date: December 16, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _FINAL_INVOICE_DATA_H_
#define _FINAL_INVOICE_DATA_H_
#include <mutex>
#include <string>
#include <column_data.h>
#include <client_data.h>
#include <invoice_data.h>
#include <business_data.h>

namespace data {
        struct final_invoice {
                public:
                        final_invoice();
                        final_invoice(const final_invoice&);
                        final_invoice(final_invoice&&);
                        final_invoice& operator= (const final_invoice&);
                        final_invoice& operator= (final_invoice&&);
                        virtual ~final_invoice();


                        [[nodiscard]] virtual bool is_valid() const;
                        void update_client_data(const data::client&);
                        void update_invoice_data(const data::invoice&);
                        void update_business_data(const data::business&);

                private:
                        void set_flag(const int&);
                        void clear_flag(const int&);
                        [[nodiscard]] bool check_flags() const;

                private:
                        std::string client_name{""};
                        std::string client_address{""};
                        std::string client_town{""};
                        std::string client_vat_number{""};

                        std::string invoice_number{""};
                        std::string invoice_order_number{""};
                        std::string invoice_job_card{""};
                        std::string invoice_date{""};
                        std::vector<data::column> description_column{};
                        std::vector<data::column> material_column{};
                        std::string invoice_description_total{""};
                        std::string invoice_material_total{""};
                        std::string invoice_grand_total{""};

                        std::string business_name{""};
                        std::string business_address{""};
                        std::string business_town{""};
                        std::string business_cellphone{""};
                        std::string business_email{""};
                        std::string business_bank_name{""};
                        std::string business_branch_code{""};
                        std::string business_account_number{""};
                        std::string business_message{""};

                        std::uint8_t flags{0x0};
                        std::mutex final_invoice_data{};
                        std::uint8_t mask{0x7};
                        enum FLAG {
                                CLIENT_DATA = 0,
                                INVOICE_DATA,
                                BUSINESS_DATA
                        };

                        enum BIT {
                                UNSET = 0,
                                SET
                        };
        };
}
#endif
