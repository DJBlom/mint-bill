/*****************************************************************************
 * @file pdf_invoice_data.h
 *
 * @brief
 *   Declares the pdf_invoice data model that aggregates client, invoice, and
 *   business information required to generate a PDF invoice document.
 *
 * @details
 *   The data::pdf_invoice structure acts as a simple aggregation layer that
 *   bundles together a validated client record, a validated invoice instance,
 *   and a validated administrative business profile. It exposes a minimal
 *   interface for setting and retrieving each component and uses a
 *   bitmask-based flag mechanism to track initialization state.
 *
 *   A mutex is used to guard modifications to internal state, allowing this
 *   composite object to be safely prepared or updated in multi-threaded
 *   environments before being passed to the PDF generation logic.
 *
 * @responsibilities
 *   Aggregate client, invoice, and business data for PDF rendering.
 *   Provide setters and getters for each aggregated component.
 *   Enforce that stored client, invoice, and business objects are valid.
 *   Track initialization state using an internal flag mask.
 *   Support thread-safe updates using a dedicated mutex.
 *
 * @notes
 *   This abstraction focuses solely on collecting the data needed to render a
 *   PDF invoice. Layout, typography, and file-generation logic are expected to
 *   be handled by separate PDF rendering components.
 *****************************************************************************/
#ifndef _PDF_INVOICE_DATA_H_
#define _PDF_INVOICE_DATA_H_
#include <mutex>
#include <string>
#include <cstdint>
#include <client_data.h>
#include <invoice_data.h>
#include <admin_data.h>

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
	[[nodiscard]] virtual data::client get_client() const;
	virtual void set_invoice(const data::invoice&);
	[[nodiscard]] virtual data::invoice get_invoice() const;
	virtual void set_business(const data::admin&);
	[[nodiscard]] virtual data::admin get_business() const;

private:
	void set_flag(const int&);
	void clear_flag(const int&);
	[[nodiscard]] bool check_flags() const;

private:
	using mask_type = std::uint8_t;

	data::client client;
	data::invoice invoice;
	data::admin admin;
	mask_type flags{0x0};
	std::mutex data_mutex{};
	mask_type mask{0x7};
	enum FLAG {
		CLIENT,
		INVOICE,
		ADMIN
	};

	enum BIT {
		CLEAR = 0,
		SET
	};
};
}
#endif
