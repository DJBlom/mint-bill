/********************************************************
 * Contents: Business info interface definition
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _BUSINESS_H_
#define _BUSINESS_H_
#include <storage.h>
#include <business_data.h>

namespace interface {
        struct business {
                public:
                        virtual ~business() = default;

                        [[nodiscard]] virtual data::business load(const interface::storage&) = 0;
                        [[nodiscard]] virtual bool save(const data::business&, const interface::storage&) = 0;
        };
}
#endif
