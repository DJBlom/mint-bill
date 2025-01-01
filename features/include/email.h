/********************************************************
 * Contents: Email definition
 * Author: Dawid J. Blom
 * Date: December 11, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _EMAIL_H_
#define _EMAIL_H_
#include <string>
#include <file.h>
#include <errors.h>
#include <curl/curl.h>
#include <email_data.h>
#include <client_data.h>
#include <business_data.h>

namespace feature {
        class email {
                public:
                        email();
                        email(const email&) = delete;
                        email(email&&) = delete;
                        email& operator = (const email&) = delete;
                        email& operator = (email&&) = delete;
                        virtual ~email();

                        [[nodiscard]] bool send(const data::email&);

                private:
                        [[nodiscard]] bool sender_setup(const data::business&);
                        [[nodiscard]] bool recipient_setup(const data::client&);

                private:
                        std::string smtp_url{"smtp://smtp.gmail.com:587"};
                        CURL *curl{nullptr};
                        curl_mime *alt{nullptr};
                        curl_mime *mime{nullptr};
                        struct curl_slist *slist{nullptr};
                        struct curl_slist *headers{nullptr};
                        struct curl_slist *recipients{nullptr};
        };
}
#endif
