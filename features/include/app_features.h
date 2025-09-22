/********************************************************
 * Contents: Abstract classes for features
 * Author: Dawid J. Blom
 * Date: June 11, 2025
 *
 * NOTE:
 *******************************************************/
#ifndef _FEATURES_H_
#define _FEATURES_H_
#include <any>
#include <memory>
#include <string>
#include <vector>
#include <email_data.h>

namespace interface {
class pdf {
public:
	virtual ~pdf() = default;

	[[nodiscard]] virtual std::string generate(const std::any&) = 0;
};
}
#endif
