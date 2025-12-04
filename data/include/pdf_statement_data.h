/*****************************************************************************
 * @file pdf_statement_data.h
 *
 * @brief
 *   Declares the pdf_statement data model that aggregates statement metadata
 *   and associated PDF invoice data required for statement PDF generation.
 *
 * @details
 *   The data::pdf_statement structure encapsulates the essential information
 *   needed to render a statement as a PDF document. It stores a statement
 *   number, date, total, the underlying statement data object, and a
 *   collection of pdf_invoice instances representing individual invoices
 *   included in the statement.
 *
 *   A bitmask-based flag mechanism is used to track initialization of each
 *   core attribute. The is_valid() function consults this flag state to
 *   determine whether the pdf_statement instance is complete and suitable for
 *   use by downstream PDF rendering logic. A mutex guards modifications to
 *   internal state, enabling safe use in multi-threaded environments.
 *
 * @responsibilities
 *   Represent statement-level metadata for PDF rendering.
 *   Aggregate statement data and related pdf_invoice objects.
 *   Provide setters and getters for all stored attributes.
 *   Track initialization state via an internal flag mask.
 *   Support thread-safe updates using a dedicated mutex.
 *
 * @notes
 *   This type focuses on data aggregation only. Layout, pagination, and other
 *   PDF formatting concerns are expected to be handled by higher-level
 *   rendering components.
 *****************************************************************************/
#ifndef _PDF_STATEMENT_DATA_H_
#define _PDF_STATEMENT_DATA_H_
#include <statement_data.h>
#include <pdf_invoice_data.h>

namespace data {
struct pdf_statement {
public:
        pdf_statement();
        pdf_statement(const pdf_statement&);
        pdf_statement(pdf_statement&&);
        pdf_statement& operator= (const pdf_statement&);
        pdf_statement& operator= (pdf_statement&&);
        virtual ~pdf_statement();

	[[nodiscard]] virtual bool is_valid() const;
	virtual void set_number(const std::string&);
	[[nodiscard]] virtual std::string get_number() const;
	virtual void set_date(const std::string&);
	[[nodiscard]] virtual std::string get_date() const;
	virtual void set_total(const std::string&);
	[[nodiscard]] virtual std::string get_total() const;
	virtual void set_statement(const data::statement&);
	[[nodiscard]] virtual data::statement get_statement() const;
	virtual void set_pdf_invoices(const std::vector<data::pdf_invoice>&);
	[[nodiscard]] virtual std::vector<data::pdf_invoice> get_pdf_invoices() const;

private:
	void set_flag(const int&);
	void clear_flag(const int&);
	[[nodiscard]] bool check_flags() const;

private:
	using mask_type = std::uint8_t;

	std::string number{""};
	std::string date{""};
	std::string total{""};
	data::statement statement_data{};
	std::vector<data::pdf_invoice> pdf_invoices{};
	std::mutex data_mutex{};
	mask_type flags{0x0};
	mask_type mask{0x1F};
	enum FLAG {
		NUMBER = 0,
		DATE,
		TOTAL,
		STATEMENT,
		PDF_INVOICE
	};

	enum BIT {
		CLEAR = 0,
		SET
	};
};
}
#endif
