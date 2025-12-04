/*****************************************************************************
 * @file    invoice_pdf.h
 *
 * @brief
 *   Declaration of the PDF invoice generator.
 *
 * @details
 *   This header declares the `feature::invoice_pdf` class, an implementation
 *   of the `interface::pdf` interface used to generate invoice PDFs for the
 *   Mint-Bill application.
 *
 *   Responsibilities:
 *     - Accept a `data::pdf_invoice` aggregate (business, client, invoice).
 *     - Lay out invoice content using Cairo / cairomm on a PDF surface:
 *         * Document header (title and top-of-page layout).
 *         * Business and client information sections.
 *         * Invoice metadata (number, date, order number, job card).
 *         * Labor and material tables with quantities, descriptions, amounts.
 *         * Totals (labor, materials, and grand total).
 *         * Payment method and optional client message.
 *     - Expose a single `generate()` API that returns the final PDF as an
 *       in-memory string buffer suitable for email attachments or file I/O.
 *
 *   Layout and rendering details:
 *     - Uses fixed page dimensions (A4 in points: 595 x 842).
 *     - Provides helper methods for:
 *         * Text placement: left-aligned, centered, and right-aligned.
 *         * Section spacing, line drawing, and page-break handling.
 *         * Column layout for quantity / description / amount fields.
 *     - Uses `utility::boundary_slicer` to wrap long item descriptions over
 *       multiple lines without exceeding column boundaries.
 *
 *   Concurrency and safety:
 *     - Maintains internal drawing state (`current_width`, `current_height`)
 *       protected by a `std::mutex` to guard against accidental concurrent
 *       access during rendering.
 *     - All drawing goes through `context_ok()` to verify the Cairo context
 *       remains in a valid state.
 *
 *   Error handling:
 *     - `generate()` returns an empty string if any step fails (e.g., Cairo
 *       surface/context creation or rendering failure).
 *     - Helper methods return `bool` to allow early bail-out when layout or
 *       drawing operations fail.
 *
 *****************************************************************************/
#ifndef _INVOICE_PDF_H_
#define _INVOICE_PDF_H_
#include <mutex>
#include <string>
#include <vector>
#include <ostream>
#include <sstream>
#include <errors.h>
#include <algorithm>
#include <app_features.h>
#include <cairo/cairo.h>
#include <client_data.h>
#include <invoice_data.h>
#include <admin_data.h>
#include <cairomm/cairomm.h>
#include <boundary_slicer.h>
#include <pdf_invoice_data.h>
#include <poppler/cpp/poppler-document.h>

namespace feature {
class invoice_pdf : public interface::pdf {
public:
	invoice_pdf();
	invoice_pdf(const invoice_pdf&) = delete;
	invoice_pdf(invoice_pdf&&) = delete;
	invoice_pdf& operator = (const invoice_pdf&) = delete;
	invoice_pdf& operator = (invoice_pdf&&) = delete;
	virtual ~invoice_pdf() override;

	[[nodiscard]] virtual std::string generate(const std::any&) override;

private:
	[[nodiscard]] bool add_header(const std::string&);
	[[nodiscard]] bool add_information(const data::pdf_invoice&);
	[[nodiscard]] bool add_invoice(const data::invoice&);
	[[nodiscard]] bool add_labor(const data::invoice&);
	[[nodiscard]] bool add_material(const data::invoice&);
	[[nodiscard]] bool add_items(const std::vector<data::column>&);
	[[nodiscard]] bool add_item_description(const data::column&);
	[[nodiscard]] bool add_grand_total(const data::invoice&);
	[[nodiscard]] bool add_payment_method(const data::admin&);
	[[nodiscard]] bool write_to_pdf(const std::string&, const double&);
	[[nodiscard]] bool write_to_pdf_in_center(const std::string&, const double&);
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
	void align_to_center(const Cairo::TextExtents&);
	void adjust_height();
	void adjust_payment_height();

private:
	const double width{595.0};
	const double height{842.0};
	double current_width{0.0};
	double current_height{0.0};
	Cairo::RefPtr<Cairo::PdfSurface> surface{};
	Cairo::RefPtr<Cairo::Context> context{};
	utility::boundary_slicer slicer{};
	std::mutex pdf_mutex{};
};
}
#endif
