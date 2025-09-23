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

namespace model {
class client: public interface::model {
public:
	virtual ~client() override;

	[[nodiscard]] virtual std::any load(const std::string&) override;
	[[nodiscard]] virtual bool save(const std::any&) override;
};
}
#endif
