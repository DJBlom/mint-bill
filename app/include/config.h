#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <string>
namespace app {
namespace config {
	constexpr int password_number{0};
	constexpr const char *password_manager_schema_name{"org.mint-bill.password"};
	constexpr const char *app_name{"org.mint-bill"};
	constexpr const char *path_to_ui_file{"gui/mint-bill.ui"};
	constexpr const char *path_to_database_file{"mint-bill.db"};
}
}
#endif
