#include <printer.h>
#include <errors.h>
#include <iostream>


gui::part::printer::printer(const std::string& _job_name, const std::vector<std::shared_ptr<poppler::document>>& _documents)
	: documents{_documents}
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
		this->print_settings->set_printer("");
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
		this->print_operation->set_allow_async(true);
		this->print_operation->set_job_name(_job_name);
		this->print_operation->set_use_full_page(true);
		this->print_operation->set_show_progress(true);
		this->print_operation->set_has_selection(false);
		this->print_operation->set_unit(Gtk::Unit::POINTS);
		this->print_operation->set_print_settings(this->print_settings);
		this->print_operation->set_default_page_setup(this->page_setup);
	}

	number_of_pages_to_print();
	this->print_operation->signal_draw_page().connect(
			sigc::mem_fun(*this, &printer::draw_page));
	this->print_operation->signal_done().connect(
			sigc::mem_fun(*this, &printer::print_operation_done));
}

bool gui::part::printer::is_connected() const
{
        std::string has_printer{this->print_settings->get_printer()};
	return !has_printer.empty();
}

bool gui::part::printer::print(const interface::operations_page& _page) const
{
	bool success{false};
	if (this->documents.empty())
	{
                syslog(LOG_CRIT, "No _data to print - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		success = true;
		this->print_operation->set_n_pages(this->total_pages);
		this->print_operation->run(Gtk::PrintOperation::Action::PREVIEW);
		_page.print_operation_notify();
	}

	return success;
}

void gui::part::printer::draw_page(const std::shared_ptr<Gtk::PrintContext>& _context, int _page_number) const
{
        for (const page_range& range : this->page_ranges)
        {
		if (range.check(_page_number) == false)
		{
			syslog(LOG_CRIT, "The _page_number is out of range, cannot print it - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
                {
                        std::shared_ptr<poppler::document> document{this->documents[range.current_document()]};
                        if (!document)
                                return;

                        auto page = document->create_page(range.local_page(_page_number));
                        if (!page)
                                return;

                        poppler::page_renderer renderer;
                        renderer.set_render_hint(poppler::page_renderer::antialiasing, true);
                        renderer.set_render_hint(poppler::page_renderer::text_antialiasing, true);

                        double dpi_x = 190;
                        double dpi_y = 290;
                        const double target_dpi = std::max(dpi_x, dpi_y);

                        auto image = renderer.render_page(page, target_dpi, target_dpi);
                        if (!image.is_valid())
                                return;

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
                        return;
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

	if (is_connected())
	{
		syslog(LOG_CRIT, "No printer is connected - "
				 "filename %s, line number %d", __FILE__, __LINE__);
	}
}

void gui::part::printer::number_of_pages_to_print()
{
        int index{0};
        this->total_pages = 0;
        this->page_ranges.clear();
        for (const std::shared_ptr<poppler::document>& document : this->documents)
        {
                if (!document)
                {
                        continue;
                }

                int page_count = document->pages();
                this->documents.push_back(document);
                this->page_ranges.push_back(page_range{this->total_pages, page_count, index});
                this->total_pages += page_count;
                ++index;
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
