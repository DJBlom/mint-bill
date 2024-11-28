/********************************************************
 * Contents: Storage interface definition
 * Author: Dawid J. Blom
 * Date: November 25, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _STORAGE_H_
#define _STORAGE_H_
#include <vector>
#include <string>
#include <unordered_map>

namespace interface {
        class storage {
                public:
                        virtual ~storage() = default;

                        [[nodiscard]] virtual std
                                ::vector<std
                                ::unordered_map<std
                                ::string, std
                                ::string>> retrieve(const std::string&) const = 0;
                        [[nodiscard]] virtual bool update() const = 0;
                        [[nodiscard]] virtual bool insert() const = 0;
        };
}
#endif
