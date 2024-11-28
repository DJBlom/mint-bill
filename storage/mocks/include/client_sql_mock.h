/********************************************************
 * Contents: Client SQL Mock class definition
 * Author: Dawid J. Blom
 * Date: November 27, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _CLIENT_SQL_MOCK_H_
#define _CLIENT_SQL_MOCK_H_
#include <storage.h>

namespace my_mock {
        class client_sql : public interface::storage {
                public:
                        virtual ~client_sql() override;

                        [[nodiscard]] virtual std
                                ::vector<std
                                ::unordered_map<std
                                ::string, std
                                ::string>> retrieve(const std::string&) const override;

                        [[nodiscard]] virtual bool update() const override;
                        [[nodiscard]] virtual bool insert() const override;
        };
}
#endif
