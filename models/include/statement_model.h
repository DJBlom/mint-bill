/********************************************************
 * Contents: Client statement declaration
 * Author: Dawid J. Blom
 * Date: June 11, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _STATEMENT_MODEL_H_
#define _STATEMENT_MODEL_H_
#include <models.h>

namespace model {
class statement: public interface::model_operations {
public:
	statement() = delete;
	explicit statement(const std::string&, const std::string&);
	statement(const statement&) = default;
	statement(statement&&) = default;
	statement& operator= (const statement&) = default;
	statement& operator= (statement&&) = default;
	virtual ~statement() override;

	[[nodiscard]] virtual std::vector<std::any> load(const std::string&) const override;
	[[nodiscard]] virtual bool save(const std::any&) const override;
	[[nodiscard]] virtual data::email prepare_for_email(const std::vector<std::any>&) const override;
	[[nodiscard]] virtual std::vector<std::string> prepare_for_print(const std::vector<std::any>&) const override;

protected:
	[[nodiscard]] virtual std::vector<std::string> convert_pdfs_to_strings(const std::vector<std::any>&) const;

private:
	std::string database_file;
	std::string database_password;
};
}
#endif
