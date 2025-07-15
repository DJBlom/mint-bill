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

	[[nodiscard]] virtual bool is_connected() const;
	[[nodiscard]] virtual bool print(const std::vector<std::shared_ptr<poppler::document>>&);

protected:
	void draw_page(const std::shared_ptr<Gtk::PrintContext>&, int) const;
	void print_operation_done(const Gtk::PrintOperation::Result&) const;
	void number_of_pages_to_print(const std::vector<std::shared_ptr<poppler::document>>&);

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
