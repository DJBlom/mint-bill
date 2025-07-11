#ifndef _PRINTER_H_
#define _PRINTER_H_
#include <gui.h>
#include <part.h>
#include <syslog.h>

namespace gui {
namespace part {
struct page_range {
public:
	page_range() = delete;
	explicit page_range(int& _start_page, int& _page_count, size_t& _document_index)
		: start_page{_start_page}, page_count{_page_count}, document_index{_document_index} {}
	page_range(const page_range&) = default;
	page_range(page_range&&) = default;
	page_range& operator= (const page_range&) = default;
	page_range& operator= (page_range&&) = default;
	virtual ~page_range() = default;

public:
	int start_page{0};
	int page_count{0};
	size_t document_index{0};
};

class printer {
public:
	printer() = delete;
	explicit printer(const std::string&);
	printer(const printer&) = delete;
	printer(printer&&) = delete;
	printer& operator= (const printer&) = delete;
	printer& operator= (printer&&) = delete;
	~printer() = default;

	[[nodiscard]] virtual bool is_connected();
	[[nodiscard]] virtual bool print(const interface::operations_page&, const std::vector<std::any>&);

protected:
	void draw_page(const std::shared_ptr<Gtk::PrintContext>&, int);
	void print_operation_done(const Gtk::PrintOperation::Result&);

private:
	std::shared_ptr<Gtk::PageSetup> page_setup{};
	std::shared_ptr<Gtk::PrintSettings> print_settings{};
        std::shared_ptr<Gtk::PrintOperation> print_operation{};
};
}
}
#endif
