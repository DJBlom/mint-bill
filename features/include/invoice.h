/********************************************************
 * Contents: Invoice interface definition
 * Author: Dawid J. Blom
 * Date: December 9, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _INVOICE_H_
#define _INVOICE_H_
#include <storage.h>
#include <vector>
#include <invoice_data.h>

namespace interface {
        class invoice {
                public:
                        virtual ~invoice() = default;

                        [[nodiscard]] virtual data::invoice load(const std::string&, const interface::storage&) = 0;
                        [[nodiscard]] virtual bool save(const data::invoice&, const interface::storage&) = 0;
                        [[nodiscard]] virtual std::vector<data::invoice> search(const std::string&, const interface::storage&) = 0;
        };
}
#endif
