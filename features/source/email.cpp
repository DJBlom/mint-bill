/********************************************************
 * Contents: Email implementation
 * Author: Dawid J. Blom
 * Date: December 11, 2024
 *
 * NOTE:
 *******************************************************/
#include <email.h>


feature::email::email() {}

feature::email::~email() {}

bool feature::email::send(const data::email& _data)
{
        if (_data.is_valid())
        {
                smtp::client client{this->curl};
                if (client.connect(_data.get_business()) == false)
                {
                        return false;
                }

                smtp::header header{this->curl};
                if (header.add(_data) == false)
                {
                        return false;
                }

                smtp::recipients recipients{this->curl};
                if (recipients.add(_data.get_client()) == false)
                {
                        return false;
                }

                smtp::parts parts{this->curl};
                if (parts.add(_data) == false)
                {
                        return false;
                }

                if(CURLE_OK != curl_easy_perform(this->curl.get()))
                {
                        return false;
                }

                return true;
        }

        return false;
}


/********************************************************
 * Contents: SMTP::Clients implementation
 * Author: Dawid J. Blom
 * Date: January 1, 2025
 *
 * NOTE:
 *******************************************************/
smtp::client::client(const std::shared_ptr<CURL>& _curl)
        : curl{_curl} {}

smtp::client::~client() {}

bool smtp::client::connect(const data::business& _business)
{
        if (_business.is_valid())
        {
                std::string username{_business.get_email()};
                if (curl_easy_setopt(this->curl.get(), CURLOPT_USERNAME, username.c_str()) != CURLE_OK)
                {
                        return false;
                }

                std::string password{_business.get_password()};
                if (curl_easy_setopt(this->curl.get(), CURLOPT_PASSWORD, password.c_str()) != CURLE_OK)
                {
                        return false;
                }

                if (curl_easy_setopt(this->curl.get(), CURLOPT_URL, this->smtp_url.c_str()) != CURLE_OK)
                {
                        return false;
                }

                if (curl_easy_setopt(this->curl.get(), CURLOPT_USE_SSL, (long)CURLUSESSL_ALL) != CURLE_OK)
                {
                        return false;
                }

                if (curl_easy_setopt(this->curl.get(), CURLOPT_MAIL_FROM, username.c_str()) != CURLE_OK)
                {
                        return false;
                }

                return true;
        }

        return false;
}


/********************************************************
 * Contents: SMTP::Header implementation
 * Author: Dawid J. Blom
 * Date: January 2, 2025
 *
 * NOTE:
 *******************************************************/
smtp::header::header(const std::shared_ptr<CURL>& _curl)
        : curl{_curl} {}

smtp::header::~header() {}

bool smtp::header::add(const data::email& _data)
{
        bool added{false};
        if (_data.is_valid())
        {
                for (const auto& receiver : generate(_data))
                {
                        this->headers.reset(curl_slist_append(this->headers.release(), receiver.c_str()));
                }

                added = !static_cast<bool> (curl_easy_setopt(this->curl.get(), CURLOPT_HTTPHEADER, this->headers.get()));
        }

        return added;
}

[[nodiscard]] std::vector<std::string> smtp::header::generate(const data::email& _data)
{
        data::client client{_data.get_client()};
        data::business business{_data.get_business()};
        std::vector<std::string> info = {
                "To: " + to_mail(client.get_email()),
                "From: " + business.get_email(),
                "Cc: " + cc_mail(client.get_email()),
                "Subject: " + _data.get_subject()
        };

        return info;
}

std::string smtp::header::to_mail(const std::string& _emails)
{
        std::vector<std::string> sliced_emails{this->slicer.slice(_emails)};
        std::string to{""};
        if (!sliced_emails.empty())
        {
                to = std::move(sliced_emails.front());
        }

        return to;
} //GCOVR_EXCL_LINE

std::string smtp::header::cc_mail(const std::string& _emails)
{
        std::vector<std::string> sliced_emails{this->slicer.slice(_emails)};
        std::string cc{""};
        if (!sliced_emails.empty())
        {
                for (const auto& email : std::ranges::subrange(sliced_emails.begin() + 1, sliced_emails.end()))
                {
                        cc += (email + " ");
                }
        }

        return cc;
} //GCOVR_EXCL_LINE


/********************************************************
 * Contents: Recipients implementation
 * Author: Dawid J. Blom
 * Date: January 1, 2025
 *
 * NOTE:
 *******************************************************/
smtp::recipients::recipients(const std::shared_ptr<CURL>& _curl)
        : curl{_curl} {}

smtp::recipients::~recipients() {}

bool smtp::recipients::add(const data::client& _data)
{
        bool added{false};
        if (_data.is_valid())
        {
                utility::word_slicer slicer{};
                std::vector<std::string> sliced_emails{slicer.slice(_data.get_email())};
                for (const auto& _email : sliced_emails)
                {
                        this->receivers.reset(curl_slist_append(this->receivers.release(), _email.c_str()));
                }

                added = !static_cast<bool> (curl_easy_setopt(this->curl.get(), CURLOPT_MAIL_RCPT, this->receivers.get()));
        }

        return added;
}


/********************************************************
 * Contents: Parts implementation
 * Author: Dawid J. Blom
 * Date: January 1, 2025
 *
 * NOTE:
 *******************************************************/
smtp::parts::parts(const std::shared_ptr<CURL>& _curl)
        : curl{_curl}, alt{curl_mime_init(curl.get()), &curl_mime_free},
          mime{curl_mime_init(curl.get()), &curl_mime_free} {}

smtp::parts::~parts() {}

bool smtp::parts::add(const data::email& _data)
{
        bool added{false};
        if (_data.is_valid())
        {
                if (text_body() == false)
                {
                        return added;
                }

                if (html_body() == false)
                {
                        return added;
                }

                if (body() == false)
                {
                        return added;
                }

                if (attachment(_data) == false)
                {
                        return added;
                }

                added = !static_cast<bool> (curl_easy_setopt(this->curl.get(), CURLOPT_MIMEPOST, mime.get()));
        }

        return added;
}

bool smtp::parts::text_body()
{
        bool success{false};
        part = curl_mime_addpart(alt.get());
        if (part)
        {
                utility::file text_file{"email.txt"};
                std::string text{text_file.read()};
                curl_mime_data(part, text.c_str(), text.length());
                curl_mime_type(part, "text/html");

                success = true;
        }

        return success;
}

bool smtp::parts::html_body()
{
        bool success{false};
        part = curl_mime_addpart(alt.get());
        if (part)
        {
                utility::file html_file{"email.html"};
                std::string html{html_file.read()};
                curl_mime_data(part, html.c_str(), html.length());
                curl_mime_type(part, "text/html");

                success = true;
        }

        return success;
}

bool smtp::parts::body()
{
        bool success{false};
        part = curl_mime_addpart(mime.get());
        if (part)
        {
                curl_mime_subparts(part, alt.release());
                curl_mime_type(part, "multipart/alternative");
                slist.reset(curl_slist_append(slist.release(), "Content-Disposition: inline"));
                curl_mime_headers(part, slist.release(), 1);

                success = true;
        }

        return success;
}

bool smtp::parts::attachment(const data::email& _data)
{
        bool success{false};
        part = curl_mime_addpart(mime.get());
        if (part && _data.is_valid())
        {
                std::string attachment_name{_data.get_subject() + ".pdf"};
                std::string pdf_data{_data.get_pdf()};
                curl_mime_data(part, pdf_data.c_str(), pdf_data.length());
                curl_mime_type(part, "application/pdf");
                curl_mime_encoder(part, "base64");
                curl_mime_filename(part, attachment_name.c_str());

                success = true;
        }

        return success;
}
