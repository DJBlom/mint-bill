#include <printer.h>
#include <errors.h>
#include <iostream>


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
		this->page_setup->set_orientation(Gtk::PageOrientation::PORTRAIT);
		this->page_setup->set_paper_size(Gtk::PaperSize(Gtk::PAPER_NAME_A4));
		this->page_setup->set_top_margin(0, Gtk::Unit::POINTS);
		this->page_setup->set_bottom_margin(0, Gtk::Unit::POINTS);
		this->page_setup->set_left_margin(0, Gtk::Unit::POINTS);
		this->page_setup->set_right_margin(0, Gtk::Unit::POINTS);
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
		this->print_settings->set_orientation(Gtk::PageOrientation::PORTRAIT);
		this->print_settings->set_paper_size(Gtk::PaperSize(Gtk::PAPER_NAME_A4));
		this->print_settings->set_use_color(false);
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
		this->print_operation->set_default_page_setup(this->page_setup);
		this->print_operation->set_unit(Gtk::Unit::POINTS);
		this->print_operation->set_has_selection(false);
		this->print_operation->set_job_name(_job_name);
		this->print_operation->set_use_full_page(true);
		this->print_operation->set_show_progress(true);
		this->print_operation->set_allow_async(true);
		this->print_operation->set_print_settings(this->print_settings);
	}

	this->print_operation->signal_draw_page().connect(
			sigc::mem_fun(*this, &printer::draw_page));
	this->print_operation->signal_done().connect(
			sigc::mem_fun(*this, &printer::print_operation_done));
}

bool gui::part::printer::is_connected()
{
	bool connected{false};
        std::string has_printer{this->print_settings->get_printer()};
        if (has_printer.empty())
        {
                syslog(LOG_CRIT, "No printer is connected - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
	else
	{
		connected = true;
	}

	return connected;
}

bool gui::part::printer::print(const interface::operations_page& _page, const std::vector<std::any>& _data)
{
	bool success{false};
	if (_data.empty())
	{
                syslog(LOG_CRIT, "No _data to print - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		if (this->is_connected() == false)
		{
			syslog(LOG_CRIT, "No printer is connected - "
					 "filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			success = true;
			// this->print_operation->set_n_pages(this->number_of_pages);
			this->print_operation->run(Gtk::PrintOperation::Action::PREVIEW);
		}
		_page.print_operation_notify();
	}

	return success;
}

void gui::part::printer::draw_page(const std::shared_ptr<Gtk::PrintContext>& _context, int _page_number)
{
	(void) _context;
	(void) _page_number;
}

void gui::part::printer::print_operation_done(const Gtk::PrintOperation::Result& _result)
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
