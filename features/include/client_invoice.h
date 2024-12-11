/********************************************************
 * Contents: Client invoice interface definition
 * Author: Dawid J. Blom
 * Date: December 9, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _CLIENT_INVOICE_H_
#define _CLIENT_INVOICE_H_
#include <invoice.h>

namespace feature {
        class invoice: public interface::invoice {
                public:
                        virtual ~invoice() override;

                        [[nodiscard]] virtual data::invoice load(const std::string&, const interface::storage&) override;
                        [[nodiscard]] virtual bool save(const data::invoice&, const interface::storage&) override;
                        [[nodiscard]] virtual std::vector<data::invoice> search(const std::string&, const interface::storage&) override;
        };
}
#endif
