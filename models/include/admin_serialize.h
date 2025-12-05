/*******************************************************************************
 * @file admin_serialize.h
 *
 * @brief Serialization helper for administrator records.
 *
 * @details
 * This header declares the `serialize::admin` class, which implements the
 * `interface::single_serialize` interface to convert raw SQLite query
 * results into a strongly-typed `data::admin` object.
 *
 * Responsibilities:
 *  - Interpret `storage::database::part::rows` returned from the database.
 *  - Map each column to the corresponding admin field (business details,
 *    contact info, banking data, and client message).
 *  - Provide a single `extract_data()` entry point that returns a fully
 *    populated `data::admin` wrapped in `std::any`.
 *
 * The file also defines SQL query strings in `sql::query` used by the admin
 * model:
 *  - `admin_usert`           – UPSERT for admin records linked to a business.
 *  - `admin_select`          – Select admin and business data by name.
 *  - `admin_no_name_select`  – Select the first available admin/business
 *                              combination without filtering by name.
 *******************************************************************************/
#ifndef _SERIALIZE_ADMIN_H_
#define _SERIALIZE_ADMIN_H_
#include <serialize.h>
#include <admin_data.h>

namespace serialize {
class admin : public interface::single_serialize {
public:
	admin() = default;
	admin(const admin&) = delete;
	admin(admin&&) = delete;
	admin& operator= (const admin&) = delete;
	admin& operator= (admin&&) = delete;
	virtual ~admin() override;

	[[nodiscard]] virtual std::any extract_data(const storage::database::part::rows&) override;

private:
	[[nodiscard]] data::admin collect_values(const storage::database::part::rows&);

private:
	enum DATA_FIELDS {
		NAME = 0,
		ADDRESS,
		AREA_CODE,
		TOWN,
		CELLPHONE,
		EMAIL,
		BANK,
		BRANCH_CODE,
		ACCOUNT_NUMBER,
		APP_PASSWORD,
		CLIENT_MESSAGE
	};
};
}

namespace sql {
namespace query {
constexpr const char* admin_usert{R"sql(
	INSERT INTO admin (
		business_id,
		bank_name,
		branch_code,
		account_number,
		app_password,
		client_message
	)
	VALUES ((SELECT business_id FROM business_details WHERE business_name = ?), ?, ?, ?, ?, ?)
	ON CONFLICT(only_one) DO UPDATE SET
		business_id    = excluded.business_id,
		bank_name      = excluded.bank_name,
		branch_code    = excluded.branch_code,
		account_number = excluded.account_number,
		app_password   = excluded.app_password,
		client_message = excluded.client_message;
)sql"};

constexpr const char* admin_select{R"sql(
	SELECT
		bd.business_name,
		bd.street,
		bd.area_code,
		bd.town_name,
		bd.contact_number,
		bd.email_address,
		a.bank_name,
		a.branch_code,
		a.account_number,
		a.app_password,
		a.client_message
	FROM admin AS a
	JOIN business_details AS bd
		ON a.business_id = bd.business_id
		WHERE bd.business_name = ?;
)sql"};

constexpr const char* admin_no_name_select{R"sql(
	SELECT
		bd.business_name,
		bd.street,
		bd.area_code,
		bd.town_name,
		bd.contact_number,
		bd.email_address,
		a.bank_name,
		a.branch_code,
		a.account_number,
		a.app_password,
		a.client_message
	FROM admin a
	JOIN business_details bd ON bd.business_id = a.business_id;
)sql"};
}
}
#endif
