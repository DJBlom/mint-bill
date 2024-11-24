/********************************************************
 * Contents: Feature interface definition
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _FEATURE_H_
#define _FEATURE_H_
#include <storage.h>

namespace interface {
        struct feature {
                public:
                        virtual ~feature() = default;

                        [[nodiscard]] virtual bool is_valid() = 0;
                        [[nodiscard]] virtual bool load(const interface::storage&) = 0;
                        [[nodiscard]] virtual bool save(const interface::storage&) = 0;
        };
}
#endif
