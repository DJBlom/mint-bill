
#ifndef _GUI_PART_H_
#define _GUI_PART_H_
#include <part.h>
#include <syslog.h>
namespace gui {
namespace part {

class dialog : public interface::dialog {
public:
        dialog() = delete;
        explicit dialog(const std::string&);
        dialog(const dialog&) = delete;
        dialog(dialog&&) = delete;
        dialog& operator= (const dialog&) = delete;
        dialog& operator= (dialog&&) = delete;
        virtual ~dialog() override;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;
        [[nodiscard]] virtual bool is_not_valid() const override;
        [[nodiscard]] virtual bool connect() const override;
        virtual void show() const override;

private:
        std::string name{""};
        std::unique_ptr<Gtk::MessageDialog> gui_dialog{};
};


class button : public interface::button {
public:
        button() = delete;
        explicit button(const std::string&);
        button(const button&) = delete;
        button(button&&) = delete;
        button& operator= (const button&) = delete;
        button& operator= (button&&) = delete;
        virtual ~button() override;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;
        [[nodiscard]] virtual bool connect(const interface::dialog&);
        virtual void disable();
        virtual void enable();


private:
        std::string name{""};
        std::unique_ptr<Gtk::Button> gui_button{};
};
}
}
#endif
