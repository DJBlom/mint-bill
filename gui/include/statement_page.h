/********************************************************
 * Contents: Statement Page class definition
 * Author: Dawid J. Blom
 * Date: February 19, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _STATEMENT_PAGE_H_
#define _STATEMENT_PAGE_H_
#include <gui.h>
#include <syslog.h>
#include <gui_parts.h>


namespace gui {
class statement_page : public interface::gui {
public:
        statement_page() = default;
        statement_page(const statement_page&) = delete;
        statement_page(statement_page&&) = delete;
        statement_page& operator= (const statement_page&) = delete;
        statement_page& operator= (statement_page&&) = delete;
        ~statement_page() override;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;

private:
        part::button save_button{"statement-save-button"};
        part::dialog save_dialog{"statement-save-button-alert"};
        part::button print_button{"statement-print-button"};
        part::dialog print_dialog{"statement-print-alert"};
        part::button email_button{"statement-email-button"};
        part::dialog email_dialog{"statement-email-alert"};
        part::statement::column_view statement_view{"statement-column-view"};
};
}


#endif
