/*******************************************************************************
 * @file    printer.h
 *
 * @brief   Declaration of the gui::part::printer class and the helper
 *          gui::part::page_range struct used for printing multi-page PDF
 *          documents through Gtk::PrintOperation.
 *
 * @details
 * The printer class provides a high-level interface for taking raw PDF data
 * (as std::string byte buffers), loading them into Poppler documents, and
 * printing them through GTK’s printing framework. It isolates the entire
 * workflow required to:
 *
 *   - Configure print settings (orientation, margins, paper size, color mode).
 *   - Configure a Gtk::PrintOperation and bind draw callbacks.
 *   - Convert in-memory PDF data into Poppler document instances.
 *   - Flatten multiple Poppler documents into a single global page sequence.
 *   - Render Poppler pages into Cairo surfaces during print callbacks.
 *
 * Usage model:
 *   1. Construct a printer with a job name.
 *   2. Call print(documents, main_window) where `documents` is a vector of PDF
 *      byte strings.
 *   3. Internally:
 *        - render_poppler_documents() loads each PDF asynchronously.
 *        - number_of_pages_to_print() builds the page_ranges table.
 *        - draw_page() renders each requested page during Gtk’s print cycle.
 *
 * Helper type — page_range:
 *   The page_range struct represents the mapping between:
 *       - A global print page index.
 *       - The PDF document to which that page belongs.
 *       - The local page index inside that document.
 *
 *   It provides:
 *       - check(page): Does this global page fall in the range?
 *       - local_page(page): Translate global → local page number.
 *       - current_document(): Return associated document index.
 *
 * Error handling:
 *   - Constructor may throw app::errors::construction if GTK objects cannot
 *     be created.
 *   - Boolean return values are used for recoverable error reporting.
 *
 * This interface is designed to be self-contained, testable, and safe for
 * asynchronous PDF loading and synchronous GTK print execution.
 *******************************************************************************/
#ifndef _PRINTER_H_
#define _PRINTER_H_
#include <gui.h>
#include <part.h>
#include <syslog.h>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-image.h>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page-renderer.h>

namespace gui {
namespace part {
struct page_range;
class printer {
public:
	printer() = delete;
	explicit printer(const std::string&);
	printer(const printer&) = delete;
	printer(printer&&) = delete;
	printer& operator= (const printer&) = delete;
	printer& operator= (printer&&) = delete;
	~printer() = default;

	[[nodiscard]] virtual bool print(const std::vector<std::string>&,
					 const std::shared_ptr<Gtk::Window>&);

protected:
	[[nodiscard]] bool render_poppler_documents(const std::vector<std::string>&);
	[[nodiscard]] bool number_of_pages_to_print();
	void draw_page(const std::shared_ptr<Gtk::PrintContext>&, int) const;
	void print_operation_done(const Gtk::PrintOperation::Result&) const;

private:
        int total_pages{0};
	std::vector<page_range> page_ranges{};
	std::shared_ptr<Gtk::PageSetup> page_setup{};
	std::shared_ptr<Gtk::PrintSettings> print_settings{};
        std::shared_ptr<Gtk::PrintOperation> print_operation{};
	std::vector<std::shared_ptr<poppler::document>> documents{};
};

struct page_range {
public:
	page_range() = delete;
	explicit page_range(int&, int&, int&);
	page_range(const page_range&) = default;
	page_range(page_range&&) = default;
	page_range& operator= (const page_range&) = default;
	page_range& operator= (page_range&&) = default;
	virtual ~page_range() = default;

	[[nodiscard]] virtual bool check(int&) const;
	[[nodiscard]] virtual int local_page(int&) const;
	[[nodiscard]] virtual int current_document() const;

private:
	int first_page{0};
	int number_of_pages{0};
	int document_index{0};
};
}
}
#endif
