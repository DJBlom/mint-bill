/********************************************************
 * Contents: Client statement declaration
 * Author: Dawid J. Blom
 * Date: June 11, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _CLIENT_STATEMENT_H_
#define _CLIENT_STATEMENT_H_
#include <vector>
#include <interfaces.h>

namespace controller {
class client_statement: public interface::operations_controller {
public:
	client_statement() = default;
	client_statement(const client_statement&) = default;
	client_statement(client_statement&&) = default;
	client_statement& operator= (const client_statement&) = default;
	client_statement& operator= (client_statement&&) = default;
	virtual ~client_statement() override;

	[[nodiscard]] virtual std::vector<std::any> load(const std::string&) const override;
	[[nodiscard]] virtual data::email prepare_for_email(const std::vector<std::any>&) const override;
	[[nodiscard]] virtual std::vector<std::string> prepare_for_print(const std::vector<std::any>&) const override;

protected:
	[[nodiscard]] virtual std::vector<std::string> convert_pdfs_to_strings(const std::vector<std::any>&) const;
};
}
#endif
