/********************************************************
 * Contents: Statement data definition
 * Author: Dawid J. Blom
 * Date: February 19, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _PDF_STATEMENT_DATA_H_
#define _PDF_STATEMENT_DATA_H_
#include <pdf_invoice_data.h>

namespace data {
struct pdf_statement {
public:
        pdf_statement();
        pdf_statement(const pdf_statement&);
        pdf_statement(pdf_statement&&);
        pdf_statement& operator= (const pdf_statement&);
        pdf_statement& operator= (pdf_statement&&);
        virtual ~pdf_statement();

	[[nodiscard]] virtual bool is_valid() const;
	virtual void set_number(const std::string&);
	[[nodiscard]] virtual std::string get_number() const;
	virtual void set_date(const std::string&);
	[[nodiscard]] virtual std::string get_date() const;
	virtual void set_total(const std::string&);
	[[nodiscard]] virtual std::string get_total() const;
	virtual void set_pdf_invoices(const std::vector<data::pdf_invoice>&);
	[[nodiscard]] virtual std::vector<data::pdf_invoice> get_pdf_invoices() const;

private:
	void set_flag(const int&);
	void clear_flag(const int&);
	[[nodiscard]] bool check_flags() const;

private:
	using mask_type = std::uint8_t;

	std::string number{""};
	std::string date{""};
	std::string total{""};
	std::vector<data::pdf_invoice> pdf_invoices{};
	std::mutex data_mutex{};
	mask_type flags{0x0};
	mask_type mask{0xF};
	enum FLAG {
		NUMBER = 0,
		DATE,
		TOTAL,
		PDF_INVOICE
	};

	enum BIT {
		CLEAR = 0,
		SET
	};
};
}
#endif
