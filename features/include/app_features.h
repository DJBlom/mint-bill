/*****************************************************************************
 * @file    features.h
 *
 * @brief
 *   Declares common feature interfaces used across the application,
 *   specifically the abstract `pdf` interface for generating PDF documents.
 *
 * @details
 *   This header defines `interface::pdf`, an abstract base class intended to
 *   standardize PDF generation functionality throughout the system.
 *   Implementations of this interface accept strongly typed data wrapped in
 *   `std::any`, allowing the caller to provide a wide range of structured
 *   inputs without tightly coupling this interface to any specific data type.
 *
 *   Key characteristics of the design:
 *
 *     - **Polymorphic Interface:**
 *       Allows multiple PDF generators (invoices, statements, summaries, etc.)
 *       to share a common API so they can be used interchangeably.
 *
 *     - **Type-Agnostic Input via `std::any`:**
 *       Supports flexible input structures while letting implementation classes
 *       perform strict type-checking internally.
 *
 *     - **Ownership and Lifetime Safety:**
 *       Uses a virtual destructor to guarantee proper cleanup through
 *       base-class pointers.
 *
 *   This abstraction is part of the `features` layer, which encapsulates
 *   optional or pluggable functionality used by the GUI controllers and
 *   underlying data models.
 *
 * @class interface::pdf
 *   @brief Abstract interface for PDF generation.
 *
 * @fn generate(const std::any&)
 *   @brief Produces a PDF based on the provided structured data.
 *   @return A `std::string` representing a file path, file name, or raw PDF
 *           content, depending on the implementation.
 *
 *****************************************************************************/
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
