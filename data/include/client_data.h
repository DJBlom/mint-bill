/********************************************************
 * Contents: Client data definition
 * Author: Dawid J. Blom
 * Date: November 26, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _CLIENT_DATA_H_
#define _CLIENT_DATA_H_
#include <mutex>
#include <regex>
#include <string>
#include <vector>
#include <cstdint>
#include <word_slicer.h>

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
                        virtual void set_statement_schedule(const std::string&);
                        [[nodiscard]] std::string get_statement_schedule() const;

                private:
                        [[nodiscard]] bool email_address_good(const std::vector<std::string>&);
                        void set_flag(const int&);
                        void clear_flag(const int&);
                        [[nodiscard]] bool check_flags() const;

                private:
                        using mask_type = std::uint8_t;

                        std::string business_name{""};
                        std::string business_address{""};
                        std::string business_area_code{""};
                        std::string business_town_name{""};
                        std::string cellphone_number{""};
                        std::string emails{""};
                        std::string vat_number{""};
                        std::string statement_schedule{""};
                        mask_type flags{0x0};
                        std::mutex client_data{};
                        mask_type mask{0xFF};
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

                        enum BIT {
                                UNSET = 0,
                                SET
                        };
        };
}
#endif
