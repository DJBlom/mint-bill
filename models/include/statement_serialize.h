/*******************************************************************************
 * @file statement_serialize.h
 *
 * @brief Declaration of the serialize::statement class, responsible for
 *        transforming raw SQL query results into data::statement domain objects.
 *
 * @details
 * The serialize::statement class implements the interface::multi_serialize
 * interface and provides the following abilities:
 *
 *   • Convert database result sets (rows) into a vector<std::any> containing
 *     fully populated data::statement objects.
 *
 *   • Use collect_values() to iterate through each row returned from the
 *     database, mapping column positions (indexed via DATA_FIELDS) to the
 *     corresponding strongly typed data::statement fields.
 *
 *   • Handle SQL types stored in variant form (strings, integers, etc.) using
 *     std::visit, ensuring safe extraction and assignment of values.
 *
 *   • Validate result sets and record critical failures using syslog when the
 *     input rows are empty or malformed.
 *
 * SQL Query Constants:
 *   The nested sql::query namespace provides the SQL statements used for:
 *     - Inserting/updating a statement record          (statement_usert)
 *     - Selecting a specific business’s statement      (statement_select)
 *     - Selecting invoices associated with a statement (statement_invoices_select)
 *     - Selecting administrative business information   (statement_admin_select)
 *
 * DATA_FIELDS enum:
 *   Maps column indices in SQL SELECT queries to semantic fields:
 *     BUSINESS_NAME, STATEMENT_ID, PERIOD_START, PERIOD_END, DATE, PAID_STATUS
 *
 * This serializer is used by model::statement to transform the storage-layer
 * output into domain-layer objects suitable for printing, emailing, and
 * business logic workflows.
 *******************************************************************************/
#ifndef _SERIALIZE_STATEMENT_H_
#define _SERIALIZE_STATEMENT_H_
#include <serialize.h>
#include <statement_data.h>

namespace serialize {
class statement : public interface::multi_serialize {
public:
	statement() = default;
	statement(const statement&) = delete;
	statement(statement&&) = delete;
	statement& operator= (const statement&) = delete;
	statement& operator= (statement&&) = delete;
	virtual ~statement() override;

	[[nodiscard]] virtual std::vector<std::any> extract_data(const storage::database::part::rows&) override;

private:
	[[nodiscard]] std::vector<data::statement> collect_values(const storage::database::part::rows&);

private:
	enum DATA_FIELDS {
		BUSINESS_NAME = 0,
		STATEMENT_ID,
		PERIOD_START,
		PERIOD_END,
		DATE,
		PAID_STATUS
	};
};
}


namespace sql {
namespace query {
constexpr const char* statement_usert{R"sql(
	INSERT INTO statement (
		business_id,
		period_start,
		period_end,
		statement_date,
		paid_status
	) VALUES (
		(SELECT c.business_id
		FROM client c
		JOIN business_details b ON b.business_id = c.business_id
		WHERE b.business_name = ?), ?, ?, ?, ?)
	ON CONFLICT (business_id, period_start, period_end) DO UPDATE SET
		paid_status    = excluded.paid_status;
)sql"};

constexpr const char* statement_select{R"sql(
	SELECT
		b.business_name,
		s.statement_id,
		s.period_start,
		s.period_end,
		s.statement_date,
		s.paid_status
	FROM statement s
	JOIN client c          ON c.business_id = s.business_id
	JOIN business_details b ON b.business_id = c.business_id
	WHERE b.business_name = ?
	ORDER BY s.period_start DESC;
)sql"};

constexpr const char* statement_invoices_select{R"sql(
	SELECT
		i.statement_id,
		i.invoice_id,
		i.order_number,
		i.job_card_number,
		i.date_created,
		i.paid_status,
		i.material_total,
		i.description_total,
		i.grand_total
	FROM invoice i
	JOIN statement s       ON s.statement_id = i.statement_id
	JOIN client c          ON c.business_id = s.business_id
	JOIN business_details b ON b.business_id = c.business_id
	WHERE b.business_name = ?
	ORDER BY s.period_start DESC, i.date_created ASC;
)sql"};

constexpr const char* statement_admin_select{R"sql(
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
