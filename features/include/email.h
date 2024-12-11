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
#include <curl/curl.h>

namespace feature {
        class email {
                public:
                        email() = default;
                        email(const email&) = default;
                        email(email&&) = default;
                        email& operator = (const email&) = default;
                        email& operator = (email&&) = default;
                        virtual ~email() = default;

                        [[nodiscard]] bool send(const std::string&);
        };
}
#endif
