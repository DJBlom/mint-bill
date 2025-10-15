
#ifndef _SERIALIZE_BUSINESS_H_
#define _SERIALIZE_BUSINESS_H_
#include <serialize.h>

namespace serialize {
class business : public interface::serialize {
public:
	business() = default;
	business(const business&) = delete;
	business(business&&) = delete;
	business& operator= (const business&) = delete;
	business& operator= (business&&) = delete;
	virtual ~business() override;

	[[nodiscard]] virtual std::any extract_data(const storage::database::part::rows&) override;
	[[nodiscard]] virtual storage::database::sql_parameters package_data(const std::any&) override;

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
		email_address  = excluded.email_address,
		contact_number = excluded.contact_number,
		street         = excluded.street,
		area_code      = excluded.area_code,
		town_name      = excluded.town_name;
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
