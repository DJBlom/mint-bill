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

#define FROM_MAIL     "<dawidjblom@gmail.com>"
#define TO_MAIL       "<dawidjblom@gmail.com>"
#define CC_MAIL       "<dawidjblom@gmail.com>"

static const char *headers_text[] = {
  "Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n"
  "To: " TO_MAIL,
  "From: " FROM_MAIL " (Example User)",
  "Cc: " CC_MAIL " (Another example User)",
  "Subject: Invoice",
  NULL
};

static const char inline_text[] =
  "This is the inline text message of the email.\r\n"
  "\r\n"
  "  It could be a lot of lines that would be displayed in an email\r\n"
  "viewer that is not able to handle HTML.\r\n";

static const char inline_html[] =
  "<html><body>\r\n"
  "<p>This is the inline <b>HTML</b> message of the email.</p>"
  "<br />\r\n"
  "<p>It could be a lot of HTML data that would be displayed by "
  "email viewers able to handle HTML.</p>"
  "</body></html>\r\n";

bool feature::email::send(const std::string& msg)
{
        if (msg.empty())
                return false;

        CURL *curl;
        CURLcode res = CURLE_OK;

        curl = curl_easy_init();
        if(curl) {
                struct curl_slist *headers = NULL;
                struct curl_slist *recipients = NULL;
                struct curl_slist *slist = NULL;
                curl_mime *mime;
                curl_mime *alt;
                curl_mimepart *part;
                const char **cpp;

                /* This is the URL for your mailserver */
                curl_easy_setopt(curl, CURLOPT_USERNAME, "dawidjblom@gmail.com");
                curl_easy_setopt(curl, CURLOPT_PASSWORD, "welj xpmv fzfi kmfj ");
                curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
                curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

                /* Note that this option is not strictly required, omitting it results in
                * libcurl sending the MAIL FROM_MAIL command with empty sender data. All
                * autoresponses should have an empty reverse-path, and should be directed
                * to the address in the reverse-path which triggered them. Otherwise,
                * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
                * details.
                */
                curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_MAIL);

                /* Add two recipients, in this particular case they correspond to the
                * To: and Cc: addressees in the header, but they could be any kind of
                * recipient. */
                recipients = curl_slist_append(recipients, TO_MAIL);
                recipients = curl_slist_append(recipients, CC_MAIL);
                curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

                /* allow one of the recipients to fail and still consider it okay */
                curl_easy_setopt(curl, CURLOPT_MAIL_RCPT_ALLOWFAILS, 1L);

                /* Build and set the message header list. */
                for(cpp = headers_text; *cpp; cpp++)
                        headers = curl_slist_append(headers, *cpp);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

                /* Build the mime message. */
                mime = curl_mime_init(curl);

                /* The inline part is an alternative proposing the html and the text
                versions of the email. */
                alt = curl_mime_init(curl);

                /* HTML message. */
                part = curl_mime_addpart(alt);
                curl_mime_data(part, inline_html, CURL_ZERO_TERMINATED);
                curl_mime_type(part, "text/html");

                /* Text message. */
                part = curl_mime_addpart(alt);
                curl_mime_data(part, inline_text, CURL_ZERO_TERMINATED);

                /* Create the inline part. */
                part = curl_mime_addpart(mime);
                curl_mime_subparts(part, alt);
                curl_mime_type(part, "multipart/alternative");
                slist = curl_slist_append(NULL, "Content-Disposition: inline");
                curl_mime_headers(part, slist, 1);

                /* Add the current source program as an attachment. */
                part = curl_mime_addpart(mime);
                std::string txt{"Poes\n\r"};
                curl_mime_data(part, msg.c_str(), msg.length());
                curl_mime_type(part, "application/pdf");
                curl_mime_encoder(part, "base64");
                curl_mime_filename(part, "Invoice.pdf");
                curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

                /* Send the message */
                res = curl_easy_perform(curl);

                /* Check for errors */
                if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

                /* Free lists. */
                curl_slist_free_all(recipients);
                curl_slist_free_all(headers);

                /* curl does not send the QUIT command until you call cleanup, so you
                * should be able to reuse this connection for additional messages
                * (setting CURLOPT_MAIL_FROM and CURLOPT_MAIL_RCPT as required, and
                * calling curl_easy_perform() again. It may not be a good idea to keep
                * the connection open for a long time though (more than a few minutes may
                * result in the server timing out the connection), and you do want to
                * clean up in the end.
                */
                curl_easy_cleanup(curl);

                /* Free multipart message. */
                curl_mime_free(mime);
        }

        std::cout << "Resutl!!!!!!!!!--- " << std::to_string(res) << std::endl;
        return ~(int)res;
}
