/********************************************************
 * Contents: Email implementation
 * Author: Dawid J. Blom
 * Date: December 11, 2024
 *
 * NOTE:
 *******************************************************/
#include <email.h>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

#define FROM_MAIL     "<dmnsstmtest@gmail.com>"
#define TO_MAIL       "<dmnsstmtest@gmail.com>"
#define CC_MAIL       "<dmnsstmtest@gmail.com>"

static const char *headers_text[] = {
  "To: " TO_MAIL,
  "From: " FROM_MAIL "",
  "Cc: " CC_MAIL "",
  "Subject: Invoice",
  NULL
};

feature::email::email()
{
        this->curl = curl_easy_init();
        if (!this->curl)
                throw app::errors::construction;

        this->mime = curl_mime_init(this->curl);
        if (!this->mime)
                throw app::errors::construction;

        this->alt = curl_mime_init(this->curl);
        if (!this->alt)
                throw app::errors::construction;
}

feature::email::~email()
{
        curl_slist_free_all(this->headers);
        curl_slist_free_all(this->recipients);
        curl_mime_free(this->mime);
        curl_easy_cleanup(this->curl);
}

bool feature::email::send(const data::email& _data)
{
        if (!_data.is_valid())
                return false;


        const char **cpp;

        if (sender_setup(_data.get_business()) == false)
                return false;

        if (recipient_setup(_data.get_client()) == false)
                return false;

        /* Note that this option is not strictly required, omitting it results in
        * libcurl sending the MAIL FROM_MAIL command with empty sender data. All
        * autoresponses should have an empty reverse-path, and should be directed
        * to the address in the reverse-path which triggered them. Otherwise,
        * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
        * details.
        */
        //std::string from{business.get_email()};

        /* Add two recipients, in this particular case they correspond to the
        * To: and Cc: addressees in the header, but they could be any kind of
        * recipient. */
        std::string pdf_data{_data.get_pdf()};


        /* allow one of the recipients to fail and still consider it okay */
        //curl_easy_setopt(this->curl, CURLOPT_MAIL_RCPT_ALLOWFAILS, 1L);

        /* Build and set the message header list. */
        for(cpp = headers_text; *cpp; cpp++)
                headers = curl_slist_append(headers, *cpp);
        curl_easy_setopt(this->curl, CURLOPT_HTTPHEADER, headers);

        /* Build the mime message. */

        /* The inline part is an alternative proposing the html and the text
        versions of the email. */

        /* Text message. */
        curl_mimepart *part{curl_mime_addpart(alt)};
        utility::file text_file{"test_file_data.txt"};
        std::string text{text_file.read()};
        curl_mime_data(part, text.c_str(), text.length());
        curl_mime_type(part, "text/html");

        /* HTML message. */
        part = curl_mime_addpart(alt);
        utility::file html_file{"test_file_data.html"};
        std::string html{html_file.read()};
        curl_mime_data(part, html.c_str(), html.length());
        curl_mime_type(part, "text/html");

        /* Create the inline part. */
        part = curl_mime_addpart(mime);
        curl_mime_subparts(part, alt);
        curl_mime_type(part, "multipart/alternative");
        slist = curl_slist_append(NULL, "Content-Disposition: inline");
        curl_mime_headers(part, slist, 1);

        /* Add the current source program as an attachment. */
        part = curl_mime_addpart(mime);
        curl_mime_data(part, pdf_data.c_str(), pdf_data.length());
        curl_mime_type(part, "application/pdf");
        curl_mime_encoder(part, "base64");
        curl_mime_filename(part, "Invoice.pdf");
        curl_easy_setopt(this->curl, CURLOPT_MIMEPOST, mime);

        /* Check for errors */
        if(CURLE_OK != curl_easy_perform(this->curl))
                fprintf(stderr, "curl_easy_perform() failed\n");


        return true;
}

bool feature::email::sender_setup(const data::business& _business)
{
        std::string username{_business.get_email()};
        if (curl_easy_setopt(this->curl, CURLOPT_USERNAME, username.c_str()) != CURLE_OK)
                return false;

        std::string password{_business.get_password()};
        if (curl_easy_setopt(this->curl, CURLOPT_PASSWORD, password.c_str()) != CURLE_OK)
                return false;

        if (curl_easy_setopt(this->curl, CURLOPT_URL, smtp_url.c_str()) != CURLE_OK)
                return false;

        if (curl_easy_setopt(this->curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL) != CURLE_OK)
                return false;

        if (curl_easy_setopt(this->curl, CURLOPT_MAIL_FROM, username.c_str()) != CURLE_OK)
                return false;

        return true;
}

bool feature::email::recipient_setup(const data::client& _client)
{
        std::vector<std::string> emails{_client.get_email()};
        for (const auto& mail : emails)
                curl_slist_append(this->recipients, mail.c_str());

        //std::string cc{client.get_email()};
        //recipients = curl_slist_append(recipients, cc.c_str());
        curl_easy_setopt(this->curl, CURLOPT_MAIL_RCPT, this->recipients);

        return true;
}
