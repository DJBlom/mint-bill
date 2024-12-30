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
#include <curl/curl.h>

namespace feature {
        class email {
                public:
                        email();
                        email(const email&) = delete;
                        email(email&&) = delete;
                        email& operator = (const email&) = delete;
                        email& operator = (email&&) = delete;
                        virtual ~email();

                        [[nodiscard]] bool send(const std::string&);
        };
}
#endif
