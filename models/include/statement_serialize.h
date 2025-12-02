
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
		statement_date = excluded.statement_date,
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
}
}
#endif
