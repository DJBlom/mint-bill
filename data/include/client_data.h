/********************************************************
 * Contents: Client data definition
 * Author: Dawid J. Blom
 * Date: November 26, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _CLIENT_DATA_H_
#define _CLIENT_DATA_H_
#include <string>
#include <cstdint>
#include <mutex>

namespace data {
        struct client {
                public:
                        client() = default;
                        client(const client& new_data);
                        client(client&&);
                        client& operator= (const client&);
                        client& operator= (client&&);
                        virtual ~client() = default;

                        [[nodiscard]] virtual bool is_valid() const;
                        virtual void set_business_name(const std::string&);
                        [[nodiscard]] std::string get_business_name() const;
                        virtual void set_business_address(const std::string&);
                        [[nodiscard]] std::string get_business_address() const;
                        virtual void set_business_area_code(const std::string&);
                        [[nodiscard]] std::string get_business_area_code() const;
                        virtual void set_business_town_name(const std::string&);
                        [[nodiscard]] std::string get_business_town_name() const;
                        virtual void set_cellphone_number(const std::string&);
                        [[nodiscard]] std::string get_cellphone_number() const;
                        virtual void set_email(const std::string&);
                        [[nodiscard]] std::string get_email() const;
                        virtual void set_vat_number(const std::string&);
                        [[nodiscard]] std::string get_vat_number() const;
                        virtual void set_statment_schedule(const std::string&);
                        [[nodiscard]] std::string get_statment_schedule() const;

                private:
                        void set_flag(const int&);
                        void clear_flag(const int&);
                        [[nodiscard]] bool check_flags() const;

                private:
                        std::string business_name{""};
                        std::string business_address{""};
                        std::string business_area_code{""};
                        std::string business_town_name{""};
                        std::string cellphone_number{""};
                        std::string email{""};
                        std::string vat_number{""};
                        std::string statment_schedule{""};
                        std::uint8_t flags{0x0};
                        std::mutex client_data{};
                        std::uint8_t mask{0xFF};
                        enum FLAG {
                                NAME = 0,
                                ADDRESS,
                                AREA_CODE,
                                TOWN,
                                CELLPHONE,
                                EMAIL,
                                VAT_NUMBER,
                                STATMENT_SCHEDULE
                        };
        };
}
#endif
