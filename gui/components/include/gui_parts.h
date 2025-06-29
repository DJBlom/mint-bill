
#ifndef _GUI_PART_H_
#define _GUI_PART_H_
#include <part.h>
#include <syslog.h>
#include <unordered_map>
#include <pdf_invoice_data.h>
#include <poppler/cpp/poppler-page.h>
#include <poppler/cpp/poppler-image.h>
#include <poppler/cpp/poppler-document.h>
#include <poppler/cpp/poppler-page-renderer.h>

namespace gui {
namespace part {
namespace statement {
namespace columns {
struct entries : public Glib::Object {
public:
        static Glib::RefPtr<entries> create();
        static Glib::RefPtr<entries> create(const std::string&,
                                            const std::string&,
                                            const std::string&,
                                            const std::string&,
                                            const std::string&);

protected:
        entries() = default;
        explicit entries(const std::string& _invoice_number,
                                const std::string& _date,
                                const std::string& _order_number,
                                const std::string& _paid_status,
                                const std::string& _price)
                : invoice_number{_invoice_number},
                  date{_date},
                  order_number{_order_number},
                  paid_status{_paid_status},
                  price{_price} {}

public:
        std::string invoice_number{""};
        std::string date{""};
        std::string order_number{""};
        std::string paid_status{""};
        std::string price{""};
};

class invoice_number : public interface::column_item {
public:
        invoice_number() = default;
        invoice_number(const invoice_number&) = delete;
        invoice_number(invoice_number&&) = delete;
        invoice_number& operator= (const invoice_number&) = delete;
        invoice_number& operator= (invoice_number&&) = delete;
        virtual ~invoice_number() override;

        [[nodiscard]] virtual bool create(const std::string&) override;
        [[nodiscard]] virtual bool is_not_valid() const override;
        [[nodiscard]] virtual Glib::RefPtr<Gtk::ColumnViewColumn> retrieve_item() const override;
        [[nodiscard]] virtual std::string retrieve_value() const override;
private:
        void setup(const Glib::RefPtr<Gtk::ListItem>&);
        void bind(const Glib::RefPtr<Gtk::ListItem>&);
        void teardown(const Glib::RefPtr<Gtk::ListItem>&);

private:
        std::string value{""};
        Glib::RefPtr<Gtk::ColumnViewColumn> column{};
        Glib::RefPtr<Gtk::SignalListItemFactory> factory{};
};

class date : public interface::column_item {
public:
        date() = default;
        date(const date&) = delete;
        date(date&&) = delete;
        date& operator= (const date&) = delete;
        date& operator= (date&&) = delete;
        virtual ~date() override;

        [[nodiscard]] virtual bool create(const std::string&) override;
        [[nodiscard]] virtual bool is_not_valid() const override;
        [[nodiscard]] virtual Glib::RefPtr<Gtk::ColumnViewColumn> retrieve_item() const override;
        [[nodiscard]] virtual std::string retrieve_value() const override;
private:
        void setup(const Glib::RefPtr<Gtk::ListItem>&);
        void bind(const Glib::RefPtr<Gtk::ListItem>&);
        void teardown(const Glib::RefPtr<Gtk::ListItem>&);

private:
        std::string value{""};
        Glib::RefPtr<Gtk::ColumnViewColumn> column{};
        Glib::RefPtr<Gtk::SignalListItemFactory> factory{};
};

class order_number : public interface::column_item {
public:
        order_number() = default;
        order_number(const order_number&) = delete;
        order_number(order_number&&) = delete;
        order_number& operator= (const order_number&) = delete;
        order_number& operator= (order_number&&) = delete;
        virtual ~order_number() override;

        [[nodiscard]] virtual bool create(const std::string&) override;
        [[nodiscard]] virtual bool is_not_valid() const override;
        [[nodiscard]] virtual Glib::RefPtr<Gtk::ColumnViewColumn> retrieve_item() const override;
        [[nodiscard]] virtual std::string retrieve_value() const override;
private:
        void setup(const Glib::RefPtr<Gtk::ListItem>&);
        void bind(const Glib::RefPtr<Gtk::ListItem>&);
        void teardown(const Glib::RefPtr<Gtk::ListItem>&);

private:
        std::string value{""};
        Glib::RefPtr<Gtk::ColumnViewColumn> column{};
        Glib::RefPtr<Gtk::SignalListItemFactory> factory{};
};

class paid_status : public interface::column_item {
public:
        paid_status() = default;
        paid_status(const paid_status&) = delete;
        paid_status(paid_status&&) = delete;
        paid_status& operator= (const paid_status&) = delete;
        paid_status& operator= (paid_status&&) = delete;
        virtual ~paid_status() override;

        [[nodiscard]] virtual bool create(const std::string&) override;
        [[nodiscard]] virtual bool is_not_valid() const override;
        [[nodiscard]] virtual Glib::RefPtr<Gtk::ColumnViewColumn> retrieve_item() const override;
        [[nodiscard]] virtual std::string retrieve_value() const override;
private:
        void setup(const Glib::RefPtr<Gtk::ListItem>&);
        void bind(const Glib::RefPtr<Gtk::ListItem>&);
        void teardown(const Glib::RefPtr<Gtk::ListItem>&);

private:
        std::string value{""};
        Glib::RefPtr<Gtk::ColumnViewColumn> column{};
        Glib::RefPtr<Gtk::SignalListItemFactory> factory{};
};

class price : public interface::column_item {
public:
        price() = default;
        price(const price&) = delete;
        price(price&&) = delete;
        price& operator= (const price&) = delete;
        price& operator= (price&&) = delete;
        virtual ~price() override;

        [[nodiscard]] virtual bool create(const std::string&) override;
        [[nodiscard]] virtual bool is_not_valid() const override;
        [[nodiscard]] virtual Glib::RefPtr<Gtk::ColumnViewColumn> retrieve_item() const override;
        [[nodiscard]] virtual std::string retrieve_value() const override;
private:
        void setup(const Glib::RefPtr<Gtk::ListItem>&);
        void bind(const Glib::RefPtr<Gtk::ListItem>&);
        void teardown(const Glib::RefPtr<Gtk::ListItem>&);

private:
        std::string value{""};
        Glib::RefPtr<Gtk::ColumnViewColumn> column{};
        Glib::RefPtr<Gtk::SignalListItemFactory> factory{};
};
}

class column_view : public interface::column_view {
public:
        column_view() = delete;
        explicit column_view(const std::string&, const std::string&);
        column_view(const column_view&) = delete;
        column_view(column_view&&) = delete;
        column_view& operator= (const column_view&) = delete;
        column_view& operator= (column_view&&) = delete;
        virtual ~column_view() override;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;
        [[nodiscard]] virtual bool is_not_valid() const override;
        [[nodiscard]] virtual bool add_column(const interface::column_item&) override;
        [[nodiscard]] virtual bool populate(const std::vector<std::any>&) override;
        [[nodiscard]] virtual bool clear() override;
        [[nodiscard]] virtual std::vector<std::any> extract() override;

private:
        std::string name{""};
        std::string vadjustment{""};
        std::unique_ptr<Gtk::ColumnView> view{};
	std::shared_ptr<Gtk::Adjustment> adjustment{};
        std::shared_ptr<Gio::ListStore<statement::columns::entries>> store{};

	enum DURATION {
		MS_30 = 30
	};
};

namespace rows{
struct invoice_pdf_entries : public Glib::Object {
public:
        static Glib::RefPtr<invoice_pdf_entries> create();
        static Glib::RefPtr<invoice_pdf_entries> create(const data::pdf_invoice&);

protected:
        invoice_pdf_entries() = default;
        explicit invoice_pdf_entries(const data::pdf_invoice& _pdf_invoice): pdf_invoice{_pdf_invoice} {}

public:
	data::pdf_invoice pdf_invoice{};
};
}

class pdf_window {
public:
	pdf_window() = default;
	explicit pdf_window(const std::string&);
	~pdf_window() = default;

	bool generate(const std::shared_ptr<poppler::document>&);

private:

	std::string title{""};
	enum WINDOW_SIZE {
		WIDTH = 595,
		HEIGHT = 515
	};

	enum A4_DIMENTIONS {
		PAGE_WIDTH = 595,
		PAGE_HEIGHT = 842
	};
};

class pdf_draw : public Gtk::DrawingArea {
public:
	pdf_draw() = delete;
	explicit pdf_draw(std::shared_ptr<poppler::document>, int);

private:
	void on_draw(const Cairo::RefPtr<Cairo::Context>&, int, int);

private:
	std::shared_ptr<poppler::document> document;
	int page_number;

	enum DPI {
		X = 190,
		Y = 290
	};

	enum WINDOW_SIZE {
		WIDTH = 595,
		HEIGHT = 515
	};

	enum A4_DIMENTIONS {
		PAGE_WIDTH = 595,
		PAGE_HEIGHT = 842
	};
};

class invoice_pdf_view : public interface::list_view {
public:
        invoice_pdf_view() = delete;
        explicit invoice_pdf_view(const std::string&, const std::string&);
        invoice_pdf_view(const invoice_pdf_view&) = delete;
        invoice_pdf_view(invoice_pdf_view&&) = delete;
        invoice_pdf_view& operator= (const invoice_pdf_view&) = delete;
        invoice_pdf_view& operator= (invoice_pdf_view&&) = delete;
        virtual ~invoice_pdf_view() override;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;
        [[nodiscard]] virtual bool is_not_valid() const override;
        [[nodiscard]] virtual bool populate(const std::vector<std::any>&) override;
        [[nodiscard]] virtual bool clear() override;
        [[nodiscard]] virtual std::vector<std::any> extract() override;

private:
	void display_invoice(uint);
	void setup(const Glib::RefPtr<Gtk::ListItem>&);
	void bind(const Glib::RefPtr<Gtk::ListItem>&);
	void teardown(const Glib::RefPtr<Gtk::ListItem>&);

private:
        std::string name{""};
        std::string vadjustment_name{""};
        std::unique_ptr<Gtk::ListView> view{};
	std::shared_ptr<Gtk::Adjustment> vadjustment{};
        std::shared_ptr<Gio::ListStore<rows::invoice_pdf_entries>> store{};

	enum DURATION {
		MS_30 = 30
	};
};
}

class search_bar : public interface::observer {
public:
	search_bar() = delete;
	explicit search_bar(const std::string&);
	search_bar(const search_bar&) = delete;
	search_bar(search_bar&&) = delete;
	search_bar& operator= (const search_bar&) = delete;
	search_bar& operator= (search_bar&&) = delete;
        virtual ~search_bar() override;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;
        [[nodiscard]] virtual bool is_not_valid() const override;
	[[nodiscard]] virtual bool subscribe(const std::string& page_name,
				      std::function<void(const std::string&)> callback) const override;

private:
	void on_search_changed(const interface::stack&);

private:
	std::string search_bar_name{""};
	std::string search_keyword{""};
	std::string previous_stack_page{""};
	std::unique_ptr<Gtk::SearchEntry> gui_search_bar{};
	mutable std::unordered_map<std::string,
		std::function<void(const std::string&)>> subscribers{};
};

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
        [[nodiscard]] virtual bool connect(const interface::dialog&) override;
        virtual void disable() override;
        virtual void enable() override;


private:
        std::string name{""};
        std::unique_ptr<Gtk::Button> gui_button{};
};

class sub_button : public interface::observer {
public:
	sub_button() = delete;
	explicit sub_button(const std::string&);
	sub_button(const sub_button&) = delete;
	sub_button(sub_button&&) = delete;
	sub_button& operator= (const sub_button&) = delete;
	sub_button& operator= (sub_button&&) = delete;
        virtual ~sub_button() override;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;
        [[nodiscard]] virtual bool is_not_valid() const override;
	[[nodiscard]] virtual bool subscribe(const std::string&,
					     std::function<void(const std::string&)>) const override;
	[[nodiscard]] virtual bool disable();
	[[nodiscard]] virtual bool enable();

private:
	void on_clicked(const interface::stack&);

private:
	std::string button_name{""};
	std::unique_ptr<Gtk::Button> button{};
	mutable std::unordered_map<std::string,
		std::function<void(const std::string&)>> subscribers{};
};
}
}
#endif
