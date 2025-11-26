/********************************************************
 * Contents: Client invoice interface definition
 * Author: Dawid J. Blom
 * Date: December 9, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _INVOICE_MODEL_H_
#define _INVOICE_MODEL_H_
#include <string>
#include <models.h>
#include <pdf_invoice_data.h>

namespace model {
class invoice: public interface::model_ops {
public:
	invoice() = delete;
	explicit invoice(const std::string&, const std::string&);
	invoice(const invoice&) = delete;
	invoice(invoice&&) = delete;
	invoice& operator=(const invoice&) = delete;
	invoice& operator=(invoice&&) = delete;
	virtual ~invoice() override;

	[[nodiscard]] virtual std::vector<std::any> load(const std::string&) const override;
	[[nodiscard]] virtual bool save(const std::any&) const override;
	[[nodiscard]] virtual data::email prepare_for_email(const std::vector<std::any>&) const override;
	[[nodiscard]] virtual std::vector<std::string> prepare_for_print(const std::vector<std::any>&) const override;


private:
	std::string database_file;
	std::string database_password;
};
}
#endif
