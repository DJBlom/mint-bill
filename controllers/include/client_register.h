/********************************************************
 * Contents: Client register interface definition
 * Author: Dawid J. Blom
 * Date: November 27, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _CLIENT_REGISTER_H_
#define _CLIENT_REGISTER_H_
#include <controllers.h>

namespace feature {
        class client: public interface::client {
                public:
                        virtual ~client() override;

                        [[nodiscard]] virtual data::client load(const std::string&) override;
                        [[nodiscard]] virtual bool save(const data::client&) override;
                        [[nodiscard]] virtual data::client search(const std::string&) override;
        };
}
#endif
