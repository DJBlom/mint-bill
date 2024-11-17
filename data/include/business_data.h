/********************************************************
 * Contents: Business data  definition
 * Author: Dawid J. Blom
 * Date: November 16, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _BUSINESS_DATA_H_
#define _BUSINESS_DATA_H_
#include <data.h>
#include <string>
#include <cstdint>
#include <mutex>

namespace data {
        struct business : public interface::data {
                public:
                        business() = default;
                        business(const business&) = default;
                        business(business&&) = default;
                        business& operator= (const business&) = default;
                        business& operator= (business&&) = default;
                        virtual ~business() = default;

                        [[nodiscard]] virtual bool is_valid() override;

                        void set_name(const std::string&);
                        [[nodiscard]] std::string get_name();
                        void set_address(const std::string&);
                        [[nodiscard]] std::string get_address();
                        void set_area_code(const std::string&);
                        [[nodiscard]] std::string get_area_code();
                        void set_town(const std::string&);
                        [[nodiscard]] std::string get_town();
                        void set_cellphone(const std::string&);
                        [[nodiscard]] std::string get_cellphone();
                        void set_email(const std::string&);
                        [[nodiscard]] std::string get_email();
                        void set_bank(const std::string&);
                        [[nodiscard]] std::string get_bank();
                        void set_branch_code(const std::string&);
                        [[nodiscard]] std::string get_branch_code();
                        void set_account_number(const std::string&);
                        [[nodiscard]] std::string get_account_number();
                        void set_client_message(const std::string&);
                        [[nodiscard]] std::string get_client_message();

                private:
                        void set_flag(const int&);
                        void clear_flag(const int&);
                        [[nodiscard]] bool check_flags();

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
                        std::mutex data;
                        std::uint16_t mask{0xFFFF};
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
        };
}
#endif
