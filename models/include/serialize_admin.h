
#ifndef _SERIALIZE_ADMIN_H_
#define _SERIALIZE_ADMIN_H_
#include <serialize.h>

namespace utility {
class serialize_admin : public interface::serialize {
public:
	serialize_admin() = default;
	serialize_admin(const serialize_admin&) = delete;
	serialize_admin(serialize_admin&&) = delete;
	serialize_admin& operator= (const serialize_admin&) = delete;
	serialize_admin& operator= (serialize_admin&&) = delete;
	virtual ~serialize_admin() override;

	[[nodiscard]] virtual storage::database::sql_parameters package_data(const std::any&) override;
	[[nodiscard]] virtual std::any extract_data(const storage::database::part::rows&) override;
};
}
#endif
