/********************************************************
 * Contents: Storage class definition
 * Author: Dawid J. Blom
 * Date: November 25, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _SQL_H_
#define _SQL_H_
#include <storage.h>

namespace storage {
        class sql : public interface::storage {
                public:
                        virtual ~sql() override;

                        [[nodiscard]] virtual std
                                ::vector<std
                                ::unordered_map<std
                                ::string, std
                                ::string>> retrieve() const override;

                        [[nodiscard]] virtual bool update() const override;
                        [[nodiscard]] virtual bool insert() const override;
        };
}
#endif
