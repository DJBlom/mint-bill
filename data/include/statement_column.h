/********************************************************
 * Contents: Statement column definition
 * Author: Dawid J. Blom
 * Date: April 4, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _STATEMENT_COLUMN_H_
#define _STATEMENT_COLUMN_H_
#include <string>
#include <cstdint>

namespace data {
        struct statement_column {
                public:
                        statement_column() = default;
                        statement_column(const statement_column&) = default;
                        statement_column(statement_column&&) = default;
                        statement_column& operator= (const statement_column&) = default;
                        statement_column& operator= (statement_column&&) = default;
                        virtual ~statement_column() = default;

                        [[nodiscard]] virtual bool is_valid();
                        virtual void set_invoice_number(const std::string&);
                        [[nodiscard]] virtual std::string get_invoice_number();

                private:
                        void set_flags(const int&);
                        void clear_flags(const int&);

                private:
                        std::string invoice_number{""};
                        std::uint8_t flags{0x0};
                        std::uint8_t mask{0x1};

                        enum FLAG {
                                INVOICE_NUMBER = 0
                        };

                        enum BIT {
                                CLEAR = 0,
                                SET
                        };
        };
}
#endif
