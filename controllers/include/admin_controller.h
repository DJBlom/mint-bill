/********************************************************
 * Contents: admin info feature definition
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _ADMIN_CONTROLLER_H_
#define _ADMIN_CONTROLLER_H_
#include <controllers.h>

namespace controller {
class admin: public interface::controller {
public:
	admin() = default;
	admin(const admin& new_data) = delete;
	admin(admin&&) = default;
	admin& operator= (const admin&) = delete;
	admin& operator= (admin&&) = delete;
	virtual ~admin() override;

	[[nodiscard]] virtual std::any load(const std::string&) override;
	[[nodiscard]] virtual bool save(const std::any&) override;
};
}
#endif
