/*******************************************************************************
 * @file models.h
 *
 * @brief Abstract interfaces defining the core model operations framework.
 *
 * @details
 * This header declares two foundational interfaces used throughout the system's
 * data-model layer. These interfaces define uniform behaviors for loading,
 * saving, and preparing model data for downstream consumers such as PDF or
 * email generation.
 *
 *   • interface::model_operations
 *       - Represents the primary contract for all data models (Admin, Client,
 *         Invoice, Statement).
 *       - Ensures every model can:
 *            • Load domain data from storage using a string key.
 *            • Save domain data using std::any-based type erasure.
 *            • Prepare email data structures for outbound messages.
 *            • Produce printable artifacts such as encoded PDFs.
 *
 *   • interface::model_register
 *       - Minimal interface intended for registry-like components that act as
 *         dispatchers or aggregators of model types.
 *       - Allows generic save/load without exposing concrete model types.
 *
 * These interfaces abstract model logic away from persistence, serialization,
 * and presentation layers, promoting clean separation of responsibilities and
 * consistent behavior across all model implementations.
 *******************************************************************************/
#ifndef _MODELS_H_
#define _MODELS_H_
#include <any>
#include <memory>
#include <string>
#include <vector>
#include <email_data.h>

namespace interface {
class model_operations {
public:
	virtual ~model_operations() = default;

	[[nodiscard]] virtual std::vector<std::any> load(const std::string&) const = 0;
	[[nodiscard]] virtual bool save(const std::any&) const = 0;
	[[nodiscard]] virtual data::email prepare_for_email(const std::vector<std::any>&) const = 0;
	[[nodiscard]] virtual std::vector<std::string> prepare_for_print(const std::vector<std::any>&) const = 0;
};

class model_register {
public:
	virtual ~model_register() = default;

	[[nodiscard]] virtual std::any load(const std::string&) = 0;
	[[nodiscard]] virtual bool save(const std::any&) = 0;
};
}
#endif
