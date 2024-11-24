/********************************************************
 * Contents: Feature interface definition
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _FEATURE_H_
#define _FEATURE_H_
#include <business_data.h>

namespace interface {
        class feature {
                public:
                        virtual ~feature() = default;

                        virtual void load(data::business&) = 0;
                        [[nodiscard]] virtual bool save(data::business&) = 0;
        };
}
#endif
