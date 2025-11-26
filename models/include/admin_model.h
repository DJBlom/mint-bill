/********************************************************
 * Contents: admin info feature definition
 * Author: Dawid J. Blom
 * Date: November 18, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _ADMIN_MODEL_H_
#define _ADMIN_MODEL_H_
#include <models.h>
#include <sqlite.h>

namespace model {
class admin: public interface::model {
public:
	admin() = delete;
	explicit admin(const std::string&, const std::string&);
	admin(const admin& new_data) = delete;
	admin(admin&&) = default;
	admin& operator= (const admin&) = delete;
	admin& operator= (admin&&) = delete;
	virtual ~admin() override;

	[[nodiscard]] virtual std::any load(const std::string&) override;
	[[nodiscard]] virtual bool save(const std::any&) override;

private:
	std::string database_file{""};
	std::string database_password{""};

	enum DATA_FIELDS {
		NAME = 0,
		EMAIL,
		CELLPHONE,
		ADDRESS,
		AREA_CODE,
		TOWN,
		BANK,
		BRANCH_CODE,
		ACCOUNT_NUMBER,
		APP_PASSWORD,
		CLIENT_MESSAGE
	};

	enum PARAMETERS {
		DETAILS = 0,
		ADMIN
	};
};
}
#endif
