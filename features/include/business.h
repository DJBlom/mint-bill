/********************************************************
 * Contents: Business feature  definition
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _BUSINESS_FEATURE_H_
#define _BUSINESS_FEATURE_H_
#include <feature.h>
#include <string>
#include <cstdint>
#include <mutex>

namespace feature {
        class business: public interface::feature {
                public:
                        business() = default;
                        business(const business& new_data);
                        business(business&&);
                        business& operator= (const business&);
                        business& operator= (business&&);
                        virtual ~business() override;

                        [[nodiscard]] virtual bool is_valid() override;
                        [[nodiscard]] virtual bool load() override;
                        [[nodiscard]] virtual bool save() override;

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
                        std::mutex business_data{};
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
