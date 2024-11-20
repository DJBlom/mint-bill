/********************************************************
 * Contents: Feature interface definition
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _FEATURE_H_
#define _FEATURE_H_
#include <data.h>

namespace interface {
        class feature {
                public:
                        virtual ~feature() = default;

                        [[nodiscard]] virtual interface::data load() = 0;
                        [[nodiscard]] virtual bool save(interface::data&) = 0;
        };
}
#endif
