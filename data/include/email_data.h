/********************************************************
 * Contents: Email data definition
 * Author: Dawid J. Blom
 * Date: December 30, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _EMAIL_DATA_H_
#define _EMAIL_DATA_H_
#include <mutex>
#include <vector>
#include <string>
#include <cstdint>
#include <client_data.h>
#include <admin_data.h>

namespace data {
        struct email {
                public:
                        email();
                        email(const email&);
                        email(email&&);
                        email& operator= (const email&);
                        email& operator= (email&&);
                        virtual ~email();

                        [[nodiscard]] virtual bool is_valid() const;
                        virtual void set_attachments(const std::vector<std::string>&);
                        [[nodiscard]] virtual std::vector<std::string> get_attachments() const;
                        virtual void set_client(const data::client&);
                        [[nodiscard]] virtual data::client get_client() const;
                        virtual void set_business(const data::admin&);
                        [[nodiscard]] virtual data::admin get_business() const;
                        virtual void set_subject(const std::string&);
                        [[nodiscard]] virtual std::string get_subject() const;

                private:
                        [[nodiscard]] bool have_file_names(const std::vector<std::string>&);
                        void set_flag(const int&);
                        void clear_flag(const int&);
                        [[nodiscard]] bool check_flags() const;

                private:
                        using mask_type = std::uint8_t;

                        std::vector<std::string> attachments{""};
                        data::client client{};
                        data::admin admin{};
                        std::string subject{""};
                        mask_type flags{0x0};
                        std::mutex email_data{};
                        mask_type mask{0xF};
                        enum FLAG {
                                PDF = 0,
                                CLIENT,
                                ADMIN,
                                SUBJECT
                        };

                        enum BIT {
                                CLEAR = 0,
                                SET
                        };
        };
}
#endif
