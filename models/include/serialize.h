

#ifndef _SERIALIZE_H_
#define _SERIALIZE_H_
#include <any>
#include <sqlite.h>

namespace interface {
class single_serialize {
public:
	virtual ~single_serialize() = default;

	[[nodiscard]] virtual std::any extract_data(const storage::database::part::rows&) = 0;
};

class multi_serialize {
public:
	virtual ~multi_serialize() = default;

	[[nodiscard]] virtual std::vector<std::any> extract_data(const storage::database::part::rows&) = 0;
};
}
#endif
