
#ifndef _GUI_PART_H_
#define _GUI_PART_H_
#include <part.h>
#include <syslog.h>
#include <unordered_map>
#include <pdf_invoice_data.h>
#include <pdf_statement_data.h>
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
        static Glib::RefPtr<entries> create(const data::invoice&);

protected:
        entries() = default;
        explicit entries(const data::invoice& _invoice) : invoice{_invoice} {}

public:
	data::invoice invoice;
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

struct statement_pdf_entries : public Glib::Object {
public:
        static Glib::RefPtr<statement_pdf_entries> create();
        static Glib::RefPtr<statement_pdf_entries> create(const data::pdf_statement&);

protected:
        statement_pdf_entries() = default;
        explicit statement_pdf_entries(const data::pdf_statement& _pdf_statement): pdf_statement{_pdf_statement} {}

public:
	data::pdf_statement pdf_statement{};
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

class statement_pdf_view : public interface::list_view_callback {
public:
        statement_pdf_view() = delete;
        explicit statement_pdf_view(const std::string&, const std::string&);
        statement_pdf_view(const statement_pdf_view&) = delete;
        statement_pdf_view(statement_pdf_view&&) = delete;
        statement_pdf_view& operator= (const statement_pdf_view&) = delete;
        statement_pdf_view& operator= (statement_pdf_view&&) = delete;
        virtual ~statement_pdf_view() override;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;
        [[nodiscard]] virtual bool is_not_valid() const override;
        [[nodiscard]] virtual bool populate(const std::vector<std::any>&) override;
        [[nodiscard]] virtual bool clear() override;
        [[nodiscard]] virtual std::vector<std::any> extract() override;
	[[nodiscard]] virtual bool double_click(std::function<void(const std::any&)>) override;
	[[nodiscard]] virtual bool single_click(std::function<void(const std::vector<std::any>&)>) override;

private:
	void edit_statement(uint);
	void selected_statement(uint, uint);
	void setup(const Glib::RefPtr<Gtk::ListItem>&);
	void bind(const Glib::RefPtr<Gtk::ListItem>&);
	void teardown(const Glib::RefPtr<Gtk::ListItem>&);

private:
        std::string name{""};
        std::string vadjustment_name{""};
        std::unique_ptr<Gtk::ListView> view{};
	std::shared_ptr<Gtk::Adjustment> vadjustment{};
        std::shared_ptr<Gio::ListStore<rows::statement_pdf_entries>> store{};
	std::function<void(const std::any&)> double_click_callback{};
	std::function<void(const std::vector<std::any>&)> single_click_callback{};

	enum DURATION {
		MS_30 = 30
	};
};
}

class search_bar : public interface::publisher {
public:
	search_bar() = delete;
	explicit search_bar(const std::string&);
	search_bar(const search_bar&) = delete;
	search_bar(search_bar&&) = delete;
	search_bar& operator= (const search_bar&) = delete;
	search_bar& operator= (search_bar&&) = delete;
        virtual ~search_bar() override;

        [[nodiscard]] virtual bool create(const Glib::RefPtr<Gtk::Builder>&) override;
	[[nodiscard]] virtual bool update(const std::string&) override;
        [[nodiscard]] virtual bool is_not_valid() const override;
	[[nodiscard]] virtual bool subscribe(const std::string&,
				      std::function<void(const std::string&)>) const override;

private:
	void on_search_changed();

private:
	std::string search_bar_name{""};
	std::string search_keyword{""};
	std::string stack_page_name{""};
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
	[[nodiscard]] virtual bool connect(const std::function<void(const int&)>&) override;
        [[nodiscard]] virtual bool show() const override;
	[[nodiscard]] virtual bool hide() const override;

private:
	void on_response(int response);

private:
        std::string name{""};
	std::function<void(const int&)> callback{};
        std::unique_ptr<Gtk::MessageDialog> gui_dialog{};
};

class sub_button : public interface::dispatcher {
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
	[[nodiscard]] virtual bool subscribe(std::function<void(void)>) override;
	[[nodiscard]] virtual bool disable();
	[[nodiscard]] virtual bool enable();

private:
	void on_clicked();

private:
	std::string button_name{""};
	std::unique_ptr<Gtk::Button> button{};
	std::function<void(void)> callback{};
};
}
}
#endif
