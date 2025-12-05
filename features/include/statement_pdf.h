/******************************************************************************
 * @file statement_pdf.h
 * @brief Declaration of the statement_pdf class for generating customer
 *        statements in PDF format.
 *
 * @details
 * This header defines the feature::statement_pdf class, an implementation of the
 * interface::pdf abstraction used to generate structured financial statements.
 * The class encapsulates PDF rendering logic using Cairo and CairoMM, providing
 * a clean API for formatting client details, account summaries, invoice lists,
 * totals, and payment information.
 *
 * Purpose:
 *  - Produce professional, consistent statement documents for customers.
 *  - Support multi-invoice summaries and account-level information.
 *  - Integrate with application data models (client, admin, pdf_statement).
 *
 * Overview of Responsibilities:
 *  - Render PDF headers, metadata blocks, invoice tables, and totals.
 *  - Format all textual sections using configurable font sizes and alignment
 *    routines (left, center, right, first/second quarter).
 *  - Create table-like structures using Cairo drawing primitives.
 *  - Manage pagination and spatial constraints to avoid visual overflow.
 *
 * Thread Safety:
 *  - A mutex ensures synchronized updates to PDF cursor positions during write
 *    operations, preventing inconsistent layout in multi-threaded contexts.
 *
 * Public API:
 *  - std::string generate(const std::any&)
 *      Converts a data::pdf_statement into a fully rendered PDF stored in memory
 *      and returned as a binary string. Handles validation, surface creation,
 *      context setup, and full layout execution.
 *
 * Internal Helpers:
 *  - add_header(), add_information(), add_statement_information(),
 *    add_statements(), add_items(), add_grand_total(), add_payment_method()
 *      These methods draw specific document sections.
 *
 *  - write_to_pdf(), write_to_pdf_in_first_quarter(), write_to_pdf_in_center(),
 *    write_to_pdf_in_second_quarter(), write_to_pdf_from_right(), etc.
 *      Handle positioning, text rendering, and formatting for each section.
 *
 *  - align_to_*(...) and adjust_height()/adjust_payment_height()
 *      Manage layout flow and prevent clipping across PDF pages.
 *
 * Error Handling:
 *  - Invalid data triggers syslog messages and aborts PDF generation.
 *  - Failures at any rendering stage cause early termination with an empty PDF.
 *
 ******************************************************************************/
#ifndef _STATEMENT_PDF_H_
#define _STATEMENT_PDF_H_
#include <mutex>
#include <string>
#include <vector>
#include <syslog.h>
#include <app_features.h>
#include <cairo/cairo.h>
#include <client_data.h>
#include <invoice_data.h>
#include <admin_data.h>
#include <cairomm/cairomm.h>
#include <boundary_slicer.h>
#include <pdf_statement_data.h>
#include <poppler/cpp/poppler-document.h>

namespace feature {
class statement_pdf : public interface::pdf {
public:
	statement_pdf();
	statement_pdf(const statement_pdf&) = delete;
	statement_pdf(statement_pdf&&) = delete;
	statement_pdf& operator = (const statement_pdf&) = delete;
	statement_pdf& operator = (statement_pdf&&) = delete;
	virtual ~statement_pdf() override;

	[[nodiscard]] std::string generate(const std::any&) override;

private:
	[[nodiscard]] bool add_header(const std::string&);
	[[nodiscard]] bool add_information(const data::client&, const data::admin&);
	[[nodiscard]] bool add_statement_information(const data::pdf_statement&);
	[[nodiscard]] bool add_statements(const data::pdf_statement&);
	[[nodiscard]] bool add_items(const std::vector<data::pdf_invoice>&);
	[[nodiscard]] bool add_grand_total(const data::pdf_statement&);
	[[nodiscard]] bool add_payment_method(const data::admin&);
	[[nodiscard]] bool write_to_pdf(const std::string&, const double&);
	[[nodiscard]] bool write_to_pdf_in_first_quarter(const std::string&, const double&);
	[[nodiscard]] bool write_to_pdf_in_center(const std::string&, const double&);
	[[nodiscard]] bool write_to_pdf_in_second_quarter(const std::string&, const double&);
	[[nodiscard]] bool write_to_pdf_from_right(const std::string&, const double&);
	[[nodiscard]] bool write_to_pdf_from_right_information(const std::string&, const double&);
	[[nodiscard]] bool draw_line();
	[[nodiscard]] bool context_ok();
	void add_new_line();
	void add_new_section();
	void align_to_left_border();
	void align_to_right_border();
	void align_information_section();
	void align_to_top_border();
	void align_to_right(const Cairo::TextExtents&);
	void align_to_right_information(const Cairo::TextExtents&);
	void align_to_first_quarter(const Cairo::TextExtents&);
	void align_to_center(const Cairo::TextExtents&);
	void align_to_second_quarter(const Cairo::TextExtents&);
	void adjust_height();
	void adjust_payment_height();

private:
	const double width{595.0};
	const double height{842.0};
	double current_width{0.0};
	double current_height{0.0};
	Cairo::RefPtr<Cairo::PdfSurface> surface{};
	Cairo::RefPtr<Cairo::Context> context{};
	utility::boundary_slicer slicer{15};
	std::mutex pdf_mutex{};
};
}
#endif
