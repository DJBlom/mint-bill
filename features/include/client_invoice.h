/********************************************************
 * Contents: Client invoice interface definition
 * Author: Dawid J. Blom
 * Date: December 9, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _CLIENT_INVOICE_H_
#define _CLIENT_INVOICE_H_
#include <email.h>
#include <invoice.h>
#include <thread>
#include <future>

namespace feature {
        class invoice: public interface::invoice {
                public:
                        virtual ~invoice() override;

                        [[nodiscard]] virtual data::invoice load(const std::string&, const interface::storage&) override;
                        [[nodiscard]] virtual bool save(const data::invoice&, const interface::storage&) override;
                        [[nodiscard]] virtual std::vector<data::invoice> search(const std::string&, const interface::storage&) override;
                        //[[nodiscard]] virtual bool send_email(const data::invoice&);
                        //virtual void send_email(const data::invoice& _data, std::function<void(bool)>);
                        [[nodiscard]] bool send_email(const data::invoice&);

                private:
                        [[nodiscard]] bool sending(const data::email&);

                private:
                        feature::email email{};
        };
}
#endif
