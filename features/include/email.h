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
#include <memory>
#include <errors.h>
#include <curl/curl.h>
#include <email_data.h>
#include <client_data.h>
#include <word_slicer.h>
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
                        const std::shared_ptr<CURL> curl{curl_easy_init(), curl_easy_cleanup};
        };
}

namespace smtp {
        class client {
                public:
                        client() = delete;
                        explicit client(const std::shared_ptr<CURL>&);
                        client(const client&) = delete;
                        client(client&&) = delete;
                        client& operator = (const client&) = delete;
                        client& operator = (client&&) = delete;
                        virtual ~client();

                        [[nodiscard]] bool connect(const data::business&);

                private:
                        std::shared_ptr<CURL> curl;
                        std::string smtp_url{"smtp://smtp.gmail.com:587"};
        };

        class header {
                public:
                        header() = delete;
                        explicit header(const std::shared_ptr<CURL>&);
                        header(const header&) = delete;
                        header(header&&) = delete;
                        header& operator = (const header&) = delete;
                        header& operator = (header&&) = delete;
                        virtual ~header();

                        [[nodiscard]] bool add(const data::email&);

                private:
                        [[nodiscard]] std::vector<std::string> generate(const data::email&);
                        [[nodiscard]] std::string to_mail(const std::string&);
                        [[nodiscard]] std::string cc_mail(const std::string&);

                private:
                        std::shared_ptr<CURL> curl;
                        utility::word_slicer slicer{};
                        std::unique_ptr<struct curl_slist, decltype(&curl_slist_free_all)> headers{
                                nullptr, curl_slist_free_all
                        };
        };

        class recipients {
                public:
                        recipients() = delete;
                        explicit recipients(const std::shared_ptr<CURL>&);
                        recipients(const recipients&) = delete;
                        recipients(recipients&&) = delete;
                        recipients& operator = (const recipients&) = delete;
                        recipients& operator = (recipients&&) = delete;
                        virtual ~recipients();

                        [[nodiscard]] bool add(const data::client&);

                private:
                        std::shared_ptr<CURL> curl;
                        std::unique_ptr<struct curl_slist, decltype(&curl_slist_free_all)> receivers{
                                nullptr, curl_slist_free_all
                        };
        };

        class parts {
                public:
                        parts() = delete;
                        explicit parts(const std::shared_ptr<CURL>&);
                        parts(const parts&) = delete;
                        parts(parts&&) = delete;
                        parts& operator = (const parts&) = delete;
                        parts& operator = (parts&&) = delete;
                        virtual ~parts();

                        [[nodiscard]] bool add(const data::email&);

                private:
                        [[nodiscard]] bool text_body();
                        [[nodiscard]] bool html_body();
                        [[nodiscard]] bool body();
                        [[nodiscard]] bool attachment(const data::email&);

                private:
                        std::shared_ptr<CURL> curl;
                        std::unique_ptr<curl_mime, decltype(&curl_mime_free)> alt;
                        std::unique_ptr<curl_mime, decltype(&curl_mime_free)> mime;
                        curl_mimepart* part{nullptr};
                        std::unique_ptr<struct curl_slist, decltype(&curl_slist_free_all)> slist{
                                nullptr, curl_slist_free_all
                        };
        };
}
#endif
