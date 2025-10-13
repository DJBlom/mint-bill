

#ifndef _SERIALIZE_H_
#define _SERIALIZE_H_
#include <any>
#include <sqlite.h>

namespace interface {
class serialize {
public:
	virtual ~serialize() = default;

	[[nodiscard]] virtual storage::database::sql_parameters package_data(const std::any&) = 0;
	[[nodiscard]] virtual std::any extract_data(const storage::database::part::rows&) = 0;
};
}
#endif
