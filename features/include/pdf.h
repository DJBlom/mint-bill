/********************************************************
* Contents: PDF definition
* Author: Dawid J. Blom
* Date: December 23, 2024
*
* NOTE:
*******************************************************/
#ifndef _PDF_H_
#define _PDF_H_
#include <any>
#include <memory>
#include <poppler/cpp/poppler-document.h>

namespace interface {
class pdf {
public:
	virtual ~pdf() = default;

	[[nodiscard]] virtual std::string generate_for_email(const std::any&) = 0;
	[[nodiscard]] virtual std::shared_ptr<poppler::document> generate_for_print(const std::any&) = 0;
};
}
#endif
