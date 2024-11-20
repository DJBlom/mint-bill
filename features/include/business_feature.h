/********************************************************
 * Contents: Business feature  definition
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _BUSINESS_FEATURE_H_
#define _BUSINESS_FEATURE_H_
#include <business_data.h>

namespace feature {
        class business {
                public:
                        business() = default;
                        business(const business&) = default;
                        business(business&&) = default;
                        business& operator= (const business&) = default;
                        business& operator= (business&&) = default;
                        ~business() = default;

                        void load(data::business&);
                        [[nodiscard]] bool save(data::business&);
        };
}
#endif
