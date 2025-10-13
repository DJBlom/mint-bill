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
#include <memory>
#include <string>
#include <vector>
#include <email_data.h>

namespace interface {
class model_ops {
public:
	virtual ~model_ops() = default;

	[[nodiscard]] virtual std::vector<std::any> load(const std::string&) const = 0;
	[[nodiscard]] virtual bool save(const std::any&) const = 0;
	[[nodiscard]] virtual data::email prepare_for_email(const std::vector<std::any>&) const = 0;
	[[nodiscard]] virtual std::vector<std::string> prepare_for_print(const std::vector<std::any>&) const = 0;
};

class model {
public:
	virtual ~model() = default;

	[[nodiscard]] virtual std::any load(const std::string&) = 0;
	[[nodiscard]] virtual bool save(const std::any&) = 0;
};

class operations_controller {
public:
	virtual ~operations_controller() = default;

	[[nodiscard]] virtual std::vector<std::any> load(const std::string&) const = 0;
	[[nodiscard]] virtual data::email prepare_for_email(const std::vector<std::any>&) const = 0;
	[[nodiscard]] virtual std::vector<std::string> prepare_for_print(const std::vector<std::any>&) const = 0;
};

// class business {
// public:
// 	virtual ~business() = default;
//
// 	[[nodiscard]] virtual data::admin load() = 0;
// 	[[nodiscard]] virtual bool save(const data::admin&) = 0;
// };
}
#endif
