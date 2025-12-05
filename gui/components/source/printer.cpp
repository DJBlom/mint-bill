/*******************************************************************************
 * @file    printer.cpp
 *
 * @brief   PDF-to-printer bridge using Gtk::PrintOperation and Poppler.
 *
 * @details
 * This module provides a small printing utility that takes raw PDF bytes
 * (as std::string objects), converts them into Poppler documents, flattens
 * them into a single page sequence, and drives a Gtk::PrintOperation to
 * render them to a printer.
 *
 * Main responsibilities:
 *   - Configure a print job:
 *       * A4 portrait PageSetup with zero top/left/right margins.
 *       * Monochrome Gtk::PrintSettings (no color).
 *       * Synchronous Gtk::PrintOperation with progress UI.
 *
 *   - Convert application-generated PDFs into Poppler documents:
 *       * render_poppler_documents():
 *           - Accepts a vector of std::string, each containing raw PDF data.
 *           - Uses std::async to load each into a poppler::document safely.
 *           - Stores resulting std::shared_ptr<poppler::document> objects in
 *             the member ‘documents’ vector.
 *
 *   - Compute combined page ranges across all input documents:
 *       * number_of_pages_to_print():
 *           - Iterates all Poppler documents and records, for each, its
 *             starting page index in the global sequence, its page count,
 *             and its index in the ‘documents’ vector.
 *           - Uses the helper struct gui::part::page_range to represent
 *             these ranges and compute the total number of pages to print.
 *
 *   - Drive the Gtk print pipeline:
 *       * print():
 *           - Validates input and main_window.
 *           - Calls render_poppler_documents() and number_of_pages_to_print().
 *           - Sets the total page count on Gtk::PrintOperation and runs the
 *             PRINT_DIALOG action so the user can confirm printer, copies,
 *             etc.
 *
 *       * draw_page():
 *           - Called by Gtk::PrintOperation for each page index.
 *           - Uses page_ranges to map a global page number to the correct
 *             poppler::document and local page index.
 *           - Renders the page via poppler::page_renderer into a Cairo
 *             ImageSurface, scales it to fit the PrintContext extents, and
 *             paints it onto the printer surface.
 *
 *       * print_operation_done():
 *           - Logs the result of the print operation (ERROR, CANCEL,
 *             IN_PROGRESS).
 *           - Logs if no printer name is set on the PrintSettings.
 *
 * Helper type:
 *   - gui::part::page_range
 *       * Represents a contiguous range of pages belonging to a single
 *         Poppler document within the global print sequence.
 *       * Members:
 *           - first_page: starting page index in the global sequence.
 *           - number_of_pages: total number of pages from this document.
 *           - document_index: index into the ‘documents’ vector.
 *       * Methods:
 *           - check(page): test whether a global page index belongs to this
 *             range.
 *           - local_page(page): translate a global page index to the
 *             document’s local page index.
 *           - current_document(): return associated document index.
 *
 * Error handling:
 *   - Constructor throws app::errors::construction if any of the core Gtk
 *     objects (PageSetup, PrintSettings, PrintOperation) cannot be created.
 *   - Most methods log critical errors via syslog when encountering invalid
 *     pointers, empty inputs, or Poppler/Gtk failures but try to keep the
 *     API boolean-based for the caller.
 ******************************************************************************/
#include <future>
#include <errors.h>
#include <algorithm>
#include <printer.h>


gui::part::printer::printer(const std::string& _job_name)
{
        this->page_setup = Gtk::PageSetup::create();
        if (!this->page_setup)
        {
                syslog(LOG_CRIT, "The page_setup is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		throw app::errors::construction;
        }
	else
	{
		this->page_setup->set_top_margin(0, Gtk::Unit::POINTS);
		this->page_setup->set_left_margin(0, Gtk::Unit::POINTS);
		this->page_setup->set_right_margin(0, Gtk::Unit::POINTS);
		this->page_setup->set_orientation(Gtk::PageOrientation::PORTRAIT);
		this->page_setup->set_paper_size(Gtk::PaperSize(Gtk::PAPER_NAME_A4));
	}

	this->print_settings = Gtk::PrintSettings::create();
	if (!this->print_settings)
	{
                syslog(LOG_CRIT, "Failed to create the print_settings - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		throw app::errors::construction;
	}
	else
	{
		this->print_settings->set_use_color(false);
		this->print_settings->set_orientation(Gtk::PageOrientation::PORTRAIT);
		this->print_settings->set_paper_size(Gtk::PaperSize(Gtk::PAPER_NAME_A4));
	}

        this->print_operation = Gtk::PrintOperation::create();
        if (!this->print_operation)
        {
                syslog(LOG_CRIT, "Failed to create the print_operation - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
		throw app::errors::construction;
        }
	else
	{
		this->print_operation->set_allow_async(false);
		this->print_operation->set_job_name(_job_name);
		this->print_operation->set_use_full_page(true);
		this->print_operation->set_show_progress(true);
		this->print_operation->set_has_selection(false);
		this->print_operation->set_unit(Gtk::Unit::POINTS);
		this->print_operation->set_print_settings(this->print_settings);
		this->print_operation->set_default_page_setup(this->page_setup);
		this->print_operation->signal_draw_page().connect(
				sigc::mem_fun(*this, &printer::draw_page));
		this->print_operation->signal_done().connect(
				sigc::mem_fun(*this, &printer::print_operation_done));
	}
}

bool gui::part::printer::print(const std::vector<std::string>& _documents, const std::shared_ptr<Gtk::Window>& _main_window)
{
	bool success{false};
	if (_documents.empty() || _main_window == nullptr)
	{
                syslog(LOG_CRIT, "No _documents to print or _main_window is null - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (render_poppler_documents(_documents) == false)
		{
			syslog(LOG_CRIT, "Failed to render poppler documents - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			if (number_of_pages_to_print() == false)
			{
				syslog(LOG_CRIT, "Failed to compute the number of pages to print - "
						 "filename %s, line number %d", __FILE__, __LINE__);
			}
			else
			{
				this->print_operation->set_n_pages(this->total_pages);
				this->print_operation->run(Gtk::PrintOperation::Action::PRINT_DIALOG, *_main_window);
				success = true;
			}
		}
	}

	return success;
}

bool gui::part::printer::render_poppler_documents(const std::vector<std::string>& _documents)
{
	bool success{false};
	if (_documents.empty())
	{
                syslog(LOG_CRIT, "There are no documents available - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		std::vector<std::future<std::shared_ptr<poppler::document>>> pdf_documents;
		std::transform(_documents.cbegin(),
				_documents.cend(),
				std::back_inserter(pdf_documents),
				[] (const std::string& _document) {
					return std::async(std::launch::async, [&_document] {
						std::vector<char> byte_vector{_document.begin(), _document.end()};
						poppler::byte_array byte_array{byte_vector};
						poppler::document* raw_doc{poppler::document::load_from_data(&byte_array)};
						return std::shared_ptr<poppler::document>(raw_doc, [](poppler::document* ptr) {
							if (ptr) {
								delete ptr;
								ptr = nullptr;
							}
						});
					});
				});

		this->documents.clear();
		for (std::future<std::shared_ptr<poppler::document>>& pdf_document : pdf_documents)
		{
			this->documents.emplace_back(pdf_document.get());
		}
		success = true;
	}

	return success;
}

bool gui::part::printer::number_of_pages_to_print()
{
	bool success{false};
	if (this->documents.empty())
	{
                syslog(LOG_CRIT, "There are no documents available - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		int index{0};
		this->total_pages = 0;
		this->page_ranges.clear();
		for (const std::shared_ptr<poppler::document>& document : this->documents)
		{
			if (document)
			{
				int page_count = document->pages();
				this->page_ranges.push_back(page_range{this->total_pages, page_count, index});
				this->total_pages += page_count;
				++index;
			}
		}
		success = true;
	}

	return success;
}

void gui::part::printer::draw_page(const std::shared_ptr<Gtk::PrintContext>& _context, int _page_number) const
{
        for (const page_range& range : this->page_ranges)
        {
		if (range.check(_page_number) == true)
                {
                        std::shared_ptr<poppler::document> document{this->documents[range.current_document()]};
                        if (!document)
				break;

                        auto page = document->create_page(range.local_page(_page_number));
                        if (!page)
				break;

                        poppler::page_renderer renderer;
                        renderer.set_render_hint(poppler::page_renderer::antialiasing, true);
                        renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true);

                        double dpi_x = 190;
                        double dpi_y = 290;
                        const double target_dpi = std::max(dpi_x, dpi_y);

                        auto image = renderer.render_page(page, target_dpi, target_dpi);
                        if (!image.is_valid())
				break;

                        auto cairo_surface = Cairo::ImageSurface::create(
                                        reinterpret_cast<unsigned char*> (image.data()), Cairo::Surface::Format::ARGB32,
                                        image.width(), image.height(),
                                        image.bytes_per_row());

                        double sx = _context->get_width() / image.width();
                        double sy = _context->get_height() / image.height();
                        double scale_factor = std::min(sx, sy);
                        auto cr = _context->get_cairo_context();
                        cr->save();
                        cr->scale(scale_factor, scale_factor);
                        cr->set_source(cairo_surface, 0, 0);
                        cr->paint();
                        cr->restore();
			break;
                }
        }
}

void gui::part::printer::print_operation_done(const Gtk::PrintOperation::Result& _result) const
{
        if (_result == Gtk::PrintOperation::Result::ERROR)
        {
                syslog(LOG_CRIT, "Failed to complete the print operation - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else if (_result == Gtk::PrintOperation::Result::CANCEL)
        {
                syslog(LOG_CRIT, "The print operation was canceled - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else if (_result == Gtk::PrintOperation::Result::IN_PROGRESS)
        {
                syslog(LOG_CRIT, "The print operation is in progress - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }

        std::string printer_name{this->print_settings->get_printer()};
	if (printer_name.empty() == true)
	{
		syslog(LOG_CRIT, "No printer is connected - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
}




/***************************************************************************************
 * page_range struct
 **************************************************************************************/
gui::part::page_range::page_range(int& _first_page, int& _number_of_pages, int& _document_index)
	: first_page{_first_page}, number_of_pages{_number_of_pages}, document_index{_document_index} {}

bool gui::part::page_range::check(int& _page_number) const
{
	return ((_page_number >= this->first_page) && (_page_number < (this->first_page + this->number_of_pages)));
}

int gui::part::page_range::local_page(int& _page_number) const
{
	return (_page_number - this->first_page);
}

int gui::part::page_range::current_document() const
{
	return this->document_index;
}
