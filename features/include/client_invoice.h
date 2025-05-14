/********************************************************
 * Contents: Client invoice interface definition
 * Author: Dawid J. Blom
 * Date: December 9, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _CLIENT_INVOICE_H_
#define _CLIENT_INVOICE_H_
#include <future>
#include <thread>
#include <email.h>
#include <invoice.h>
#include <pdf_invoice_data.h>
#include <poppler/cpp/poppler-document.h>

namespace feature {
        class invoice: public interface::invoice {
                public:
                        virtual ~invoice() override;

                        [[nodiscard]] virtual data::invoice load(const std::string&, const interface::storage&) override;
                        [[nodiscard]] virtual bool save(const data::invoice&, const interface::storage&) override;
                        [[nodiscard]] virtual std::vector<data::invoice> search(const std::string&, const interface::storage&) override;
                        [[nodiscard]] bool send_email(const std::vector<data::invoice>&);
                        [[nodiscard]] std::vector<data::pdf_invoice> create_pdf_to_print(const std::vector<data::invoice>&);

                private:
                        [[nodiscard]] std::future<bool> sending(const data::email&);

                private:
                        feature::email email{};
        };
}
#endif
