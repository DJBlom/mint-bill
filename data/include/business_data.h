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
                        business(const business& new_data)
                                : name{new_data.name}, address{new_data.address}, area_code{new_data.area_code},
                                  town{new_data.town}, cellphone{new_data.cellphone}, email{new_data.email},
                                  bank{new_data.bank}, branch_code{new_data.branch_code}, account_number{new_data.account_number},
                                  client_message{new_data.client_message}, flags{new_data.flags}, business_data{}, mask{new_data.mask}{}
                        business(business&&) = default;
                        business& operator= (const business& new_data)
                        {
                                business temp {new_data};
                                std::swap(temp, *this);
                                return *this;
                        }

                        business& operator= (business&& new_data)
                        {
                                std::swap(name, new_data.name);
                                std::swap(address, new_data.address);
                                std::swap(area_code, new_data.area_code);
                                std::swap(town, new_data.town);
                                std::swap(cellphone, new_data.cellphone);
                                std::swap(email, new_data.email);
                                std::swap(bank, new_data.bank);
                                std::swap(branch_code, new_data.branch_code);
                                std::swap(account_number, new_data.account_number);
                                std::swap(client_message, new_data.client_message);
                                std::swap(flags, new_data.flags);
                                std::swap(mask, new_data.mask);

                                return *this;
                        }
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
