/********************************************************
 * Contents: Statement column decleration
 * Author: Dawid J. Blom
 * Date: April 4, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _STATEMENT_COLUMN_H_
#define _STATEMENT_COLUMN_H_
#include <mutex>
#include <string>
#include <cstdint>

namespace data {
struct statement_column {
public:
        statement_column() = default;
        statement_column(const statement_column&);
        statement_column(statement_column&&);
        statement_column& operator= (const statement_column&);
        statement_column& operator= (statement_column&&);
        virtual ~statement_column() = default;

        [[nodiscard]] virtual bool is_valid();
        virtual void set_invoice_number(const std::string&);
        [[nodiscard]] virtual std::string get_invoice_number();
        virtual void set_date(const std::string&);
        [[nodiscard]] virtual std::string get_date();
        virtual void set_order_number(const std::string&);
        [[nodiscard]] virtual std::string get_order_number();
        virtual void set_paid_status(const std::string&);
        [[nodiscard]] virtual std::string get_paid_status();
        virtual void set_price(const std::string&);
        [[nodiscard]] virtual std::string get_price();

private:
        bool check_flags();
        void set_flag(const int&);
        void clear_flag(const int&);

private:
        using mask_type = std::uint8_t;

        std::string invoice_number{""};
        std::string date{""};
        std::string order_number{""};
        std::string paid_status{""};
        std::string price{""};
        mask_type flags{0x0};
        std::mutex data_mutex{};
        mask_type mask{0x1F};

        enum FLAG {
                INVOICE_NUMBER = 0,
                DATE,
                ORDER_NUMBER,
                PAID_STATUS,
                PRICE
        };

        enum BIT {
                CLEAR = 0,
                SET
        };
};
}
#endif
