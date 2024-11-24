/********************************************************
 * Contents: Business feature  definition
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _BUSINESS_FEATURE_H_
#define _BUSINESS_FEATURE_H_
#include <feature.h>

namespace feature {
        class business: public interface::feature {
                public:
                        business() = default;
                        business(const business&) = default;
                        business(business&&) = default;
                        business& operator= (const business&) = default;
                        business& operator= (business&&) = default;
                        virtual ~business() override;

                        virtual void load(data::business&) override;
                        [[nodiscard]] virtual bool save(data::business&) override;
        };
}
#endif
