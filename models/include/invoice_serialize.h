
#ifndef _SERIALIZE_INVOICE_H_
#define _SERIALIZE_INVOICE_H_
#include <serialize.h>
#include <column_data.h>

namespace serialize {
class invoice : public interface::multi_serialize {
public:
	invoice() = default;
	invoice(const invoice&) = delete;
	invoice(invoice&&) = delete;
	invoice& operator= (const invoice&) = delete;
	invoice& operator= (invoice&&) = delete;
	virtual ~invoice() override;

	[[nodiscard]] virtual storage::database::sql_parameters package_data(const std::any&) override;
	[[nodiscard]] virtual std::vector<std::any> extract_data(const storage::database::part::rows&) override;

private:
	[[nodiscard]] std::vector<data::invoice> collect_values(const storage::database::part::rows&);

private:
	enum DATA_FIELDS {
		BUSINESS_NAME = 0,
		INVOICE_NUMBER,
		ORDER_NUMBER,
		JOB_CARD_NUMBER,
		INVOICE_DATE,
		PAID_STATUS,
		MATERIAL_TOTAL,
		DESCRIPTION_TOTAL,
		GRAND_TOTAL
	};
};

class labor {
public:
	labor() = default;
	labor(const labor&) = delete;
	labor(labor&&) = delete;
	labor& operator= (const labor&) = delete;
	labor& operator= (labor&&) = delete;
	virtual ~labor() = default;

	[[nodiscard]] virtual std::vector<data::column> extract_data(const storage::database::part::rows&);
	[[nodiscard]] virtual storage::database::sql_parameters package_data(const data::column&,
									     const data::invoice&);

private:
	[[nodiscard]] std::vector<data::column> collect_values(const storage::database::part::rows&);

private:
	enum DATA_FIELDS {
		QUANTITY = 0,
		DESCRIPTION,
		AMOUNT,
		ROW_NUMBER,
		IS_DESCRIPTION
	};
};
}


namespace sql {
namespace query {
constexpr const char* invoice_usert{R"sql(
	INSERT INTO invoice (
		invoice_id,
		business_id,
		order_number,
		job_card_number,
		date_created,
		paid_status,
		material_total,
		description_total,
		grand_total
	)
	VALUES (?,
		(SELECT c.business_id
			FROM client c
			JOIN business_details b ON b.business_id = c.business_id
			WHERE b.business_name = ?),
		?, ?, ?, ?, ?, ?, ?
	)
	ON CONFLICT (invoice_id) DO UPDATE SET
		order_number      = excluded.order_number,
		job_card_number   = excluded.job_card_number,
		date_created      = excluded.date_created,
		paid_status       = excluded.paid_status,
		material_total    = excluded.material_total,
		description_total = excluded.description_total,
		grand_total       = excluded.grand_total;
)sql"};

constexpr const char* invoice_admin_select{R"sql(
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

constexpr const char* invoice_client_select{R"sql(
	SELECT
		b.business_name,
		b.email_address,
		b.contact_number,
		b.street,
		b.area_code,
		b.town_name,
		c.vat_number,
		c.statement_schedule
	FROM client c
	JOIN business_details b ON b.business_id = c.business_id
	WHERE b.business_name = ?;
)sql"};

constexpr const char* invoice_select{R"sql(
	SELECT
		b.business_name,
		i.invoice_id,
		i.order_number,
		i.job_card_number,
		i.date_created,
		i.paid_status,
		i.material_total,
		i.description_total,
		i.grand_total
	FROM invoice i
	JOIN business_details b ON b.business_id = i.business_id
	WHERE b.business_name = ?;
)sql"};

constexpr const char* labor_usert{R"sql(
	INSERT INTO labor (
		invoice_id,
		line_number,
		is_description,
		quantity,
		description,
		amount
	)
	VALUES (
		(SELECT i.invoice_id
			FROM invoice i
			JOIN business_details b ON b.business_id = i.business_id
			WHERE b.business_name   = ?
			AND i.order_number    = ?
			AND i.job_card_number = ?),
			?, ?, ?, ?, ?
	)
	ON CONFLICT (invoice_id, line_number, is_description) DO UPDATE SET
		quantity    = excluded.quantity,
		description = excluded.description,
		amount      = excluded.amount;
)sql"};

constexpr const char* description_labor_select{R"sql(
	SELECT
		quantity,
		description ,
		amount,
		line_number,
		is_description
	FROM labor
	WHERE invoice_id = ? AND is_description = 1
	ORDER BY is_description, line_number;
)sql"};

constexpr const char* material_labor_select{R"sql(
	SELECT
		quantity,
		description ,
		amount
	FROM labor
	WHERE invoice_id = ? AND is_description = 0
	ORDER BY is_description, line_number;
)sql"};
}
}
#endif
