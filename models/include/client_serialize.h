
#ifndef _SERIALIZE_CLIENT_H_
#define _SERIALIZE_CLIENT_H_
#include <serialize.h>
#include <client_data.h>

namespace serialize {
class client : public interface::single_serialize {
public:
	client() = default;
	client(const client&) = delete;
	client(client&&) = delete;
	client& operator= (const client&) = delete;
	client& operator= (client&&) = delete;
	virtual ~client() override;

	[[nodiscard]] virtual std::any extract_data(const storage::database::part::rows&) override;

private:
	[[nodiscard]] data::client collect_values(const storage::database::part::rows&);

private:
	enum DATA_FIELDS {
		NAME = 0,
		EMAIL,
		CELLPHONE,
		ADDRESS,
		AREA_CODE,
		TOWN,
		VAT_NUMBER,
		STATEMENT_SCHEDULE
	};
};
}


namespace sql {
namespace query {

constexpr const char *client_usert{R"sql(
		INSERT INTO client (
			business_id,
			vat_number,
			statement_schedule
		)
		VALUES ((SELECT business_id FROM business_details WHERE business_name = ?), ?, ?)
		ON CONFLICT DO UPDATE SET
			vat_number	   = excluded.vat_number,
			statement_schedule = excluded.statement_schedule;
)sql"};

constexpr const char* client_select{R"sql(
	SELECT
		bd.business_name,
		bd.email_address,
		bd.contact_number,
		bd.street,
		bd.area_code,
		bd.town_name,
		c.vat_number,
		c.statement_schedule
	FROM business_details bd
	LEFT JOIN client c
		ON bd.business_id = c.business_id
	WHERE bd.business_name = ?;
)sql"};
}
}
#endif
