/********************************************************
 * Contents: Client statement declaration
 * Author: Dawid J. Blom
 * Date: June 11, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _CLIENT_STATEMENT_H_
#define _CLIENT_STATEMENT_H_
#include <interfaces.h>

namespace feature {
class client_statement: public interface::feature {
public:
	client_statement() = default;
	client_statement(const client_statement&) = default;
	client_statement(client_statement&&) = default;
	client_statement& operator= (const client_statement&) = default;
	client_statement& operator= (client_statement&&) = default;
	virtual ~client_statement() override;

	[[nodiscard]] virtual std::any load(const std::string&) const override;
};
}
#endif
