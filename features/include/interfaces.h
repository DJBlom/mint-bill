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

namespace interface {
class feature {
public:
	virtual ~feature() = default;

	[[nodiscard]] virtual std::any load(const std::string&) const = 0;
};
}
#endif
