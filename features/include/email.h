/*****************************************************************************
 * @file    email.h
 *
 * @brief
 *   Declares email–related features and SMTP helper classes built on libcurl.
 *
 * @details
 *   This header defines the high–level `feature::email` façade and a set of
 *   lower–level SMTP helper classes in the `smtp` namespace that together
 *   provide a robust, testable email–sending pipeline for the application.
 *
 *   The design separates responsibilities into four main components:
 *
 *     - feature::email
 *       Public façade used by the rest of the application. It owns a shared
 *       libcurl handle and exposes a single `send(const data::email&)` method
 *       that orchestrates SMTP connection, header setup, recipient handling,
 *       and MIME body construction.
 *
 *     - smtp::client
 *       Encapsulates SMTP connection configuration. It applies server URL,
 *       authentication, and protocol–specific libcurl options required to
 *       connect to the outbound mail server (e.g., Gmail’s SMTP endpoint).
 *
 *     - smtp::header
 *       Builds and attaches RFC–compliant email headers (From, To, Cc, Subject,
 *       etc.) using `curl_slist`. It uses `utility::word_slicer` to break long
 *       header fields (such as subject lines) into safe segments where needed.
 *
 *     - smtp::recipients
 *       Manages recipient addressing (To/Cc/Bcc) derived from `data::client`
 *       information. It configures libcurl with the final recipient list via
 *       a `curl_slist` owned by this component.
 *
 *     - smtp::parts
 *       Constructs the MIME body using `curl_mime`, including:
 *         * Plain–text body
 *         * HTML body (optionally templated via `update_dom`)
 *         * One or more file attachments sourced from `data::email`
 *
 *   Ownership and lifetime:
 *     - All SMTP helpers receive a `std::shared_ptr<CURL>` so they operate on
 *       the same underlying libcurl easy handle.
 *     - Smart pointers (`std::unique_ptr` with custom deleters) are used for
 *       libcurl lists and MIME structures to guarantee correct cleanup.
 *
 *   This module does not perform business–logic validation of email content;
 *   instead it assumes `data::email`, `data::client`, and `data::admin` have
 *   already been validated by the data layer and focuses solely on reliable
 *   transport and formatting.
 *
 *****************************************************************************/
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
#include <admin_data.h>


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

                        [[nodiscard]] bool connect(const data::admin&);

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
                        [[nodiscard]] bool text_body(const data::email&);
                        [[nodiscard]] bool html_body(const data::email&);
                        [[nodiscard]] bool body();
                        [[nodiscard]] bool attachment(const data::email&);
                        [[nodiscard]] std::string update_dom(const std::string&, const std::string&, const std::string&);

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
