/**
 * @file        password_manager_tests.cpp
 * @brief       CppUTest suite for `feature::password_manager` (libsecret adapter).
 *
 * @details
 * These tests exercise the *front-door* behavior of the password manager:
 *  - construction validation (non-empty UTF-8 schema name),
 *  - parameter validation for `store_password(...)` and `lookup_password(...)`,
 *  - successful queuing of asynchronous store/lookup requests.
 *
 * The suite deliberately focuses on immediate return semantics (i.e., whether the request
 * can be queued) rather than on the eventual asynchronous completion result. Completion
 * is handled inside the implementation via GLib/GIO callbacks; verifying those requires
 * running a GLib main loop until completion and observing side effects (e.g., a latch or
 * captured value).
 *
 * ### Runtime model
 * - Each test’s `setup()` briefly spins a GLib main loop to ensure a default main context
 *   exists and to allow any pending GLib initialization to settle. The tests themselves
 *   assert the boolean return values from the public API (queueing/validation).
 * - The final test wires a callback to capture the looked-up password, but does not assert
 *   on it because asynchronous delivery depends on a running GLib main context and an
 *   available Secret Service instance.
 *
 * ### What is covered
 * - `create_password_manager`: throws `app::errors::construction` on empty schema name.
 * - `store_password_in_the_secret_service_fail`: rejects empty secret and invalid integer bounds.
 * - `store_password_in_the_secret_service_success`: accepts well-formed input and queues request.
 * - `lookup_password_in_the_secret_service_fail`: rejects null/empty callback and invalid bounds.
 * - `lookup_password_looked_up_function_fail`: accepts well-formed input and queues lookup.
 * - `lookup_password_in_the_secret_service_success`: queues lookup with a concrete callback.
 *
 * ### Prerequisites
 * - GLib and libsecret available at runtime. For end-to-end completion, a Secret Service
 *   (e.g., GNOME Keyring) must be running on the session DBus.
 *
 * ### Building & Running (example)
 * @code
 * # CMake: link against CppUTest, glib-2.0, gobject-2.0, secret-1
 * ctest -R password_manager_test
 * @endcode
 *
 * ### Notes & caveats
 * - Secrets used in tests may be persisted in the default keyring collection; avoid
 *   production values and consider cleaning up test entries as needed.
 * - The `password_manager` implementation uses a static callback for lookup delivery;
 *   avoid running tests in parallel that perform concurrent lookups unless the design
 *   is refactored to eliminate shared state.
 */
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <glib.h>
#include <limits>
#include <errors.h>
#include <iostream>
#include <password_manager.h>
extern "C"
{
}


/**********************************TEST LIST************************************
 * 1) Store a password in the secret-service. (Done)
 * 2) Retrieve a password from the secret-service. (Done)
 ******************************************************************************/
TEST_GROUP(password_manager_test)
{
	GMainLoop *loop{nullptr};
	feature::password_manager password_manager{"org.mint-bill.password"};
	void setup()
	{
		loop = g_main_loop_new(nullptr, FALSE);
		g_timeout_add_seconds(1, [](gpointer data) -> gboolean {
				GMainLoop *loop = static_cast<GMainLoop*>(data);
				g_main_loop_quit(loop);
				return G_SOURCE_REMOVE;
			}, loop);
		g_main_loop_run(loop);
		g_main_loop_unref(loop);
	}

	void teardown()
	{
	}
};

TEST(password_manager_test, create_password_manager)
{
	CHECK_THROWS(app::errors, feature::password_manager{""});
}

TEST(password_manager_test, store_password_in_the_secret_service_fail)
{
	bool expected{false};
	CHECK_EQUAL(expected, password_manager.store_password("", 2));
	CHECK_EQUAL(expected, password_manager.store_password("password", std::numeric_limits<int>::max()));
	CHECK_EQUAL(expected, password_manager.store_password("password", std::numeric_limits<int>::min()));
}

TEST(password_manager_test, store_password_in_the_secret_service_success)
{
	bool expected{true};
	bool result{password_manager.store_password("password", 1)};

	CHECK_EQUAL(expected, result);
}

TEST(password_manager_test, lookup_password_in_the_secret_service_fail)
{
	bool expected{false};
	CHECK_EQUAL(expected, password_manager.lookup_password(nullptr, 2));
	CHECK_EQUAL(expected, password_manager.lookup_password([] (const std::string& _password) -> bool {
						return _password.empty();
					}, std::numeric_limits<int>::max()));
	CHECK_EQUAL(expected, password_manager.lookup_password([] (const std::string& _password) -> bool {
						return _password.empty();
					}, std::numeric_limits<int>::min()));
}

TEST(password_manager_test, lookup_password_looked_up_function_fail)
{
	bool expected{true};
	CHECK_EQUAL(expected, password_manager.lookup_password([] (const std::string& _password) -> bool {
						return _password.empty();
					}, 2));
}

static std::string password;
bool get_password(const std::string& _password)
{
	password = _password;
	return _password.empty();
}

TEST(password_manager_test, lookup_password_in_the_secret_service_success)
{
	bool expected{true};
	bool result{password_manager.lookup_password(get_password, 1)};

	CHECK_EQUAL(expected, result);
}
