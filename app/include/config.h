/*****************************************************************************
 * @file config.h
 *
 * @brief
 *   Centralized application configuration constants for the Mint-Bill
 *   application. This header provides compile-time values used throughout
 *   the program for identifying schemas, file paths, and key application
 *   metadata.
 *
 * @details
 *   The configuration namespace encapsulates all immutable settings required
 *   by the application at startup or during runtime initialisation. These
 *   include:
 *
 *     * The password index used by the password manager.
 *     * The GLib/GSettings schema name used for secure credential storage.
 *     * The application name identifier.
 *     * The relative path to the UI definition file.
 *     * The filename of the SQLite database used for storing records.
 *
 *   These constants eliminate magic strings within the codebase and provide
 *   a single authoritative source for configuring core behaviours.
 *
 * @namespace app::config
 *   Defines an isolated namespace to prevent naming collisions and to keep
 *   configuration values organized and easily discoverable.
 *
 * @note
 *   All values are compile-time constants and therefore cannot be changed at
 *   runtime.
 *
 *****************************************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <string>
namespace app {
namespace config {
	constexpr int password_number{0};
	constexpr const char *password_manager_schema_name{"org.mint-bill.password"};
	constexpr const char *app_name{"org.app.mint-bill"};
	constexpr const char *path_to_ui_file{"gui/mint-bill.ui"};
	constexpr const char *path_to_database_file{"mint-bill.db"};
}
}
#endif
