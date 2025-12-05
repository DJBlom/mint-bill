/*******************************************************************************
 * @file serialize.h
 *
 * @brief Abstract interfaces for transforming SQL query results into domain
 *        data structures.
 *
 * @details
 * This header defines two core serialization interfaces used throughout the
 * data-layer of the system. Their purpose is to convert raw SQL rows returned
 * from the storage engine into strongly typed domain objects using type erasure
 * (`std::any`) for flexibility.
 *
 *   • interface::single_serialize
 *       - Contract for serializers that return a single data object.
 *       - Intended for models where a query yields exactly one logical entity
 *         (e.g., Admin, Business details).
 *
 *   • interface::multi_serialize
 *       - Contract for serializers that return multiple objects.
 *       - Used for models where queries may produce a list of rows representing
 *         multiple domain items (e.g., Invoices, Statements, Columns).
 *
 * Implementations of these interfaces isolate the logic for mapping SQL query
 * results (`storage::database::part::rows`) into domain-specific C++ types,
 * ensuring that:
 *
 *   • Database schemas remain decoupled from application models.
 *   • Model and data classes do not depend on SQL structures directly.
 *   • The serialization layer can evolve independently from query execution.
 *
 *******************************************************************************/
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
