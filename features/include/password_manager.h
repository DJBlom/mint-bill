#ifndef _PASSWORD_MANAGER_H_
#define _PASSWORD_MANAGER_H_

#include <string>
#include <limits>
#include <errors.h>
#include <syslog.h>
#include <functional>
#include <libsecret/secret.h>
namespace feature {
class password_manager {
public:
	password_manager() = delete;
	explicit password_manager(const std::string&);
	password_manager(const password_manager&) = delete;
	password_manager(password_manager&&) = delete;
	password_manager& operator= (const password_manager&) = delete;
	password_manager& operator= (password_manager&&) = delete;
	virtual ~password_manager();

	[[nodiscard]] virtual bool store_password(const std::string&, const int&);
	[[nodiscard]] virtual std::string lookup_password(const int&);

private:
	std::string schema_name{};
	SecretSchema schema{};
};
}
#endif
