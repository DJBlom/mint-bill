/*******************************************************************************
 * @file business_serialize.h
 *
 * @brief Serialization helper for business detail records.
 *
 * @details
 * This header declares the `serialize::business` class, which implements the
 * `interface::single_serialize` interface to convert raw SQLite query results
 * into a `data::business` object.
 *
 * Responsibilities:
 *  - Interpret `storage::database::part::rows` returned from the database.
 *  - Map each column (name, address, area code, town, phone, email) into a
 *    structured `data::business` instance.
 *  - Provide a single `extract_data()` entry point that returns the populated
 *    object wrapped in `std::any`.
 *
 * The file also defines SQL query strings in `sql::query` used by the business
 * model:
 *  - `business_details_usert`  – UPSERT for core business details.
 *  - `business_details_select` – Select a single business by name.
 *
 * This keeps schema and SQL details localized to the serialization layer and
 * allows higher-level code to operate on domain objects instead of raw rows.
 *******************************************************************************/
#ifndef _SERIALIZE_BUSINESS_H_
#define _SERIALIZE_BUSINESS_H_
#include <serialize.h>

namespace serialize {
class business : public interface::single_serialize {
public:
	business() = default;
	business(const business&) = delete;
	business(business&&) = delete;
	business& operator= (const business&) = delete;
	business& operator= (business&&) = delete;
	virtual ~business() override;

	[[nodiscard]] virtual std::any extract_data(const storage::database::part::rows&) override;

private:
	[[nodiscard]] std::vector<std::string> collect_values(const storage::database::part::rows&);

private:
	enum DATA_FIELDS {
		NAME = 0,
		ADDRESS,
		AREA_CODE,
		TOWN,
		CELLPHONE,
		EMAIL
	};
};
}

namespace sql {
namespace query {
constexpr const char* business_details_usert{R"sql(
	INSERT INTO business_details (
		business_name,
		street,
		area_code,
		town_name,
		contact_number,
		email_address
	)
	VALUES (?, ?, ?, ?, ?, ?)
	ON CONFLICT DO UPDATE SET
		business_name  = excluded.business_name,
		street         = excluded.street,
		area_code      = excluded.area_code,
		town_name      = excluded.town_name,
		contact_number = excluded.contact_number,
		email_address  = excluded.email_address;
)sql"};

constexpr const char* business_details_select{R"sql(
	SELECT  business_name,
		street,
		area_code,
		town_name,
		contact_number,
		email_address
	FROM business_details
		WHERE business_name = ?;
	)sql"};
}
}
#endif
