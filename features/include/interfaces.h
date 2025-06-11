/********************************************************
 * Contents: Abstract classes for features
 * Author: Dawid J. Blom
 * Date: June 11, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _INTERFACES_H_
#define _INTERFACES_H_
#include <any>
#include <string>
#include <storage.h>

namespace interface {
class feature {
public:
	virtual ~feature() = default;

	[[nodiscard]] virtual std::any load(const std::string&, const interface::storage&) = 0;
	[[nodiscard]] virtual bool save(const std::any&, const interface::storage&) = 0;
	[[nodiscard]] virtual std::any search(const std::string&, const interface::storage&) = 0;
};
}
#endif
