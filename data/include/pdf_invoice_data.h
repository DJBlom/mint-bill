/********************************************************
 * Contents: PDF invoice data definition
 * Author: Dawid J. Blom
 * Date: December 23, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _PDF_INVOICE_DATA_H_
#define _PDF_INVOICE_DATA_H_
#include <mutex>
#include <string>
#include <cstdint>
#include <client_data.h>
#include <invoice_data.h>
#include <business_data.h>

namespace data {
        struct pdf_invoice {
                public:
                        pdf_invoice();
                        pdf_invoice(const pdf_invoice&);
                        pdf_invoice(pdf_invoice&&);
                        pdf_invoice& operator= (const pdf_invoice&);
                        pdf_invoice& operator= (pdf_invoice&&);
                        virtual ~pdf_invoice();

                        [[nodiscard]] virtual bool is_valid() const;
                        virtual void set_client(const data::client&);
                        [[nodiscard]] data::client get_client() const;
                        virtual void set_invoice(const data::invoice&);
                        [[nodiscard]] data::invoice get_invoice() const;
                        virtual void set_business(const data::business&);
                        [[nodiscard]] data::business get_business() const;

                private:
                        void set_flag(const int&);
                        void clear_flag(const int&);
                        [[nodiscard]] bool check_flags() const;

                private:
                        using mask_type = std::uint8_t;

                        data::client client;
                        data::invoice invoice;
                        data::business business;
                        mask_type flags{0x0};
                        std::mutex data_mutex{};
                        mask_type mask{0x7};
                        enum FLAG {
                                CLIENT,
                                INVOICE,
                                BUSINESS
                        };

                        enum BIT {
                                CLEAR = 0,
                                SET
                        };
        };
}
#endif
