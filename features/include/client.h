/********************************************************
 * Contents: Client abstract definition
 * Author: Dawid J. Blom
 * Date: November 27, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _CLIENT_H_
#define _CLIENT_H_
#include <storage.h>
#include <client_data.h>

namespace interface {
        class client {
                public:
                        virtual ~client() = default;

                        [[nodiscard]] virtual data::client load(const std::string&, const interface::storage&) = 0;
                        [[nodiscard]] virtual bool save(const data::client&, const interface::storage&) = 0;
                        [[nodiscard]] virtual data::client search(const std::string&, const interface::storage&) = 0;
        };
}
#endif
