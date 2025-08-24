/********************************************************
 * Contents: Business info feature definition
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _BUSINESS_INFO_H_
#define _BUSINESS_INFO_H_
#include <interfaces.h>

namespace feature {
        class business: public interface::business {
                public:
                        business() = default;
                        business(const business& new_data) = delete;
                        business(business&&) = default;
                        business& operator= (const business&) = delete;
                        business& operator= (business&&) = delete;
                        virtual ~business() override;

                        [[nodiscard]] virtual data::business load(const interface::storage&) override;
                        [[nodiscard]] virtual bool save(const data::business&, const interface::storage&) override;
        };
}
#endif
