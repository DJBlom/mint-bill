/********************************************************
 * Contents: Invoice column definition
 * Author: Dawid J. Blom
 * Date: December 8, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _INVOICE_COLUMN_H_
#define _INVOICE_COLUMN_H_
#include <string>
#include <cstdint>
#include <mutex>

namespace data {
        struct column {
                public:
                        column();
                        column(const column& new_data);
                        column(column&&);
                        column& operator= (const column&);
                        column& operator= (column&&);
                        virtual ~column();

                        [[nodiscard]] virtual bool is_valid() const;
                        void set_quantity(const unsigned int&);
                        [[nodiscard]] unsigned int get_quantity();
                        void set_description(const std::string&);
                        [[nodiscard]] std::string get_description();
                        void set_amount(const double&);
                        [[nodiscard]] double get_amount();

                private:
                        void set_flag(const int&);
                        void clear_flag(const int&);
                        [[nodiscard]] bool check_flags() const;

                private:
                        unsigned int quantity{0};
                        std::string description{""};
                        double amount{0.0};
                        std::uint8_t flags{0x0};
                        std::mutex column_data{};
                        std::uint8_t mask{0x7};

                        enum FLAG {
                                QUANTITY = 0,
                                DESCRIPTION,
                                AMOUNT
                        };

                        enum BIT {
                                UNSET = 0,
                                SET
                        };
        };
}
#endif
