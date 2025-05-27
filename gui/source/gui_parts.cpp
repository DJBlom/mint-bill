#include <gui_parts.h>
/***************************************************************************
 * Dialog
 **************************************************************************/
gui::part::dialog::dialog(const std::string& _name) : name{_name}
{
        name.shrink_to_fit();
}

gui::part::dialog::~dialog() {}

bool gui::part::dialog::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        bool success{false};
        if (!_ui_builder)
        {
                syslog(LOG_CRIT, "The UI builder is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
                this->gui_dialog = std::unique_ptr<Gtk::MessageDialog>{
                        _ui_builder->get_widget<Gtk::MessageDialog>(this->name)};
                if (this->gui_dialog)
                {
                        success = true;
                }
        }

        return success;
}

bool gui::part::dialog::is_not_valid() const
{
        return (!this->gui_dialog);
}

bool gui::part::dialog::connect() const
{
        bool success{false};
        if (!this->gui_dialog)
        {
                syslog(LOG_CRIT, "The dialog is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
                success = true;
                this->gui_dialog->signal_response().connect([this] (int response) {
                        switch (response)
                        {
                                case GTK_RESPONSE_YES:
                                        this->gui_dialog->hide();
                                        break;
                                case GTK_RESPONSE_NO:
                                        this->gui_dialog->hide();
                                        break;
                                default:
                                        this->gui_dialog->hide();
                                        break;
                        }
                });
        }

        return success;
}

void gui::part::dialog::show() const
{
        this->gui_dialog->show();
}








/***************************************************************************
 * Button
 **************************************************************************/
gui::part::button::button(const std::string& _name) : name{_name}
{
        name.shrink_to_fit();
}

gui::part::button::~button() {}

bool gui::part::button::create(const Glib::RefPtr<Gtk::Builder>& _ui_builder)
{
        bool created{false};
        if (!_ui_builder)
        {
                syslog(LOG_CRIT, "The UI builder is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
                this->gui_button = std::unique_ptr<Gtk::Button>{
                        _ui_builder->get_widget<Gtk::Button>(this->name)};
                if (this->gui_button != nullptr)
                {
                        created = true;
                }
        }

        return created;
}

bool gui::part::button::connect(const interface::dialog& _dialog)
{
        bool success{false};
        if (_dialog.is_not_valid())
        {
                syslog(LOG_CRIT, "The dialog is not valid - "
                                 "filename %s, line number %d", __FILE__, __LINE__);
        }
        else
        {
                success = true;
                this->gui_button->signal_clicked().connect([&] () {
                        _dialog.show();
                });
        }

        return success;
}

void gui::part::button::disable()
{
        this->gui_button->set_sensitive(false);
}

void gui::part::button::enable()
{
        this->gui_button->set_sensitive(true);
}
