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
#include <interfaces.h>
#include <pdf_invoice_data.h>
#include <poppler/cpp/poppler-document.h>

namespace feature {
	class client_invoice: public interface::operations_controller {
                public:
                        virtual ~client_invoice() override;

			[[nodiscard]] virtual std::vector<std::any> load(const std::string&) const override;
			[[nodiscard]] virtual data::email prepare_for_email(const std::vector<std::any>&) const override;
			[[nodiscard]] virtual std::vector<std::string> prepare_for_print(const std::vector<std::any>&) const override;

                        [[nodiscard]] virtual bool save(const data::invoice&, const interface::storage&);
                        [[nodiscard]] virtual std::vector<data::invoice> search(const std::string&, const interface::storage&);

                private:
                        feature::email email{};
        };
}
#endif
