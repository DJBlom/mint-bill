/********************************************************
 * Contents: Invoice data definition
 * Author: Dawid J. Blom
 * Date: December 4, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _INVOICE_DATA_H_
#define _INVOICE_DATA_H_
#include <string>
#include <cstdint>
#include <mutex>
#include <vector>
#include <column_data.h>

namespace data {
        struct invoice {
                public:
                        invoice();
                        invoice(const invoice& new_data);
                        invoice(invoice&&);
                        invoice& operator= (const invoice&);
                        invoice& operator= (invoice&&);
                        virtual ~invoice();

                        [[nodiscard]] virtual bool is_valid() const;
                        virtual void set_business_name(const std::string&);
                        [[nodiscard]] std::string get_business_name() const;
                        virtual void set_invoice_number(const unsigned int&);
                        [[nodiscard]] unsigned int get_invoice_number() const;
                        virtual void set_invoice_date(const std::string&);
                        [[nodiscard]] std::string get_invoice_date() const;
                        virtual void set_job_card_number(const std::string&);
                        [[nodiscard]] std::string get_job_card_number() const;
                        virtual void set_order_number(const std::string&);
                        [[nodiscard]] std::string get_order_number() const;
                        virtual void set_description_column(const std::vector<data::column>&);
                        [[nodiscard]] std::vector<data::column> get_description_column() const;
                        virtual void set_material_column(const std::vector<data::column>&);
                        [[nodiscard]] std::vector<data::column> get_material_column() const;

                private:
                        void set_flag(const int&);
                        void clear_flag(const int&);
                        [[nodiscard]] bool check_flags() const;

                private:
                        std::string business_name{""};
                        unsigned int invoice_number{0};
                        std::string invoice_date{""};
                        std::string job_card_number{""};
                        std::string order_number{""};
                        std::vector<data::column> description_column{};
                        std::vector<data::column> material_column{};
                        std::uint8_t flags{0x0};
                        std::mutex invoice_data{};
                        std::uint8_t mask{0x7F};
                        enum FLAG {
                                NAME = 0,
                                NUMBER,
                                DATE,
                                JOB_CARD,
                                ORDER_NUMBER,
                                DESCRIPTION_COL,
                                MATERIAL_COL
                        };

                        enum BIT {
                                UNSET = 0,
                                SET
                        };
        };
}
#endif
