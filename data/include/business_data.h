/********************************************************
 * Contents: Business data definition
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _BUSINESS_DATA_H_
#define _BUSINESS_DATA_H_
#include <string>
#include <cstdint>
#include <mutex>

namespace data {
        struct business {
                public:
                        business() = default;
                        business(const business& new_data);
                        business(business&&);
                        business& operator= (const business&);
                        business& operator= (business&&);
                        virtual ~business() = default;

                        [[nodiscard]] virtual bool is_valid() const;
                        virtual void set_name(const std::string&);
                        [[nodiscard]] std::string get_name() const;
                        virtual void set_address(const std::string&);
                        [[nodiscard]] std::string get_address() const;
                        virtual void set_area_code(const std::string&);
                        [[nodiscard]] std::string get_area_code() const;
                        virtual void set_town(const std::string&);
                        [[nodiscard]] std::string get_town() const;
                        virtual void set_cellphone(const std::string&);
                        [[nodiscard]] std::string get_cellphone() const;
                        virtual void set_email(const std::string&);
                        [[nodiscard]] std::string get_email() const;
                        virtual void set_bank(const std::string&);
                        [[nodiscard]] std::string get_bank() const;
                        virtual void set_branch_code(const std::string&);
                        [[nodiscard]] std::string get_branch_code() const;
                        virtual void set_account_number(const std::string&);
                        [[nodiscard]] std::string get_account_number() const;
                        virtual void set_client_message(const std::string&);
                        [[nodiscard]] std::string get_client_message() const;

                private:
                        void set_flag(const int&);
                        void clear_flag(const int&);
                        [[nodiscard]] bool check_flags() const;

                private:
                        std::string name{""};
                        std::string address{""};
                        std::string area_code{""};
                        std::string town{""};
                        std::string cellphone{""};
                        std::string email{""};
                        std::string bank{""};
                        std::string branch_code{""};
                        std::string account_number{""};
                        std::string client_message{""};
                        std::uint16_t flags{0x0};
                        std::mutex business_data{};
                        std::uint16_t mask{0x3FF};
                        enum FLAG {
                                NAME = 0,
                                ADDRESS,
                                AREA_CODE,
                                TOWN,
                                CELLPHONE,
                                EMAIL,
                                BANK,
                                BRANCH_CODE,
                                ACCOUNT_NUMBER,
                                CLIENT_MESSAGE
                        };

                        enum BIT {
                                UNSET = 0,
                                SET
                        };
        };
}
#endif
