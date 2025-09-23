/********************************************************
 * Contents: Client register interface definition
 * Author: Dawid J. Blom
 * Date: November 27, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _CLIENT_MODEL_H_
#define _CLIENT_MODEL_H_
#include <models.h>
#include <sqlite.h>
#include <string>

namespace model {
class client: public interface::model {
public:
	client() = delete;
	explicit client(const std::string&, const std::string&);
	client(const client&) = delete;
	client(client&&) = delete;
	client& operator= (const client&) = delete;
	client& operator= (client&&) = delete;
	virtual ~client() override;

	[[nodiscard]] virtual std::any load(const std::string&) override;
	[[nodiscard]] virtual bool save(const std::any&) override;

private:
	using details = std::vector<std::vector<storage::database::param_values>>;

	std::string database_file{""};
	std::string database_password{""};

	enum PARAMETERS {
		DETAILS = 0,
		CLIENT
	};
};
}
#endif
