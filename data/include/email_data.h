/*****************************************************************************
 * @file email_data.h
 *
 * @brief
 *   Declares the email data model that aggregates client, business, subject,
 *   and attachment information into a single logical email payload.
 *
 * @details
 *   The data::email structure encapsulates all metadata required to construct
 *   an outbound email within the application domain. It stores a list of
 *   attachment file names, a validated client object, a validated administrative
 *   business object, and a subject line. A bitmask-based flag mechanism tracks
 *   initialization of each core component, enabling is_valid() to provide a
 *   concise completeness check.
 *
 *   A mutex is used to guard modifications to internal state, supporting
 *   thread-safe updates when email objects are composed or modified from
 *   multiple threads.
 *
 * @responsibilities
 *   Aggregate client, business, subject, and attachment data for an email.
 *   Provide setters and getters for all email-related attributes.
 *   Enforce basic validity constraints on inputs and dependent objects.
 *   Track initialization state using an internal flag mask.
 *   Support thread-safe updates via a per-instance mutex.
 *
 * @notes
 *   This abstraction focuses on email metadata only; transport, formatting,
 *   and delivery responsibilities are expected to be handled by higher-level
 *   components such as an email composer or mail transport layer.
 *****************************************************************************/
#ifndef _EMAIL_DATA_H_
#define _EMAIL_DATA_H_
#include <mutex>
#include <vector>
#include <string>
#include <cstdint>
#include <client_data.h>
#include <admin_data.h>

namespace data {
struct email {
public:
	email();
	email(const email&);
	email(email&&);
	email& operator= (const email&);
	email& operator= (email&&);
	virtual ~email();

	[[nodiscard]] virtual bool is_valid() const;
	virtual void set_attachments(const std::vector<std::string>&);
	[[nodiscard]] virtual std::vector<std::string> get_attachments() const;
	virtual void set_client(const data::client&);
	[[nodiscard]] virtual data::client get_client() const;
	virtual void set_business(const data::admin&);
	[[nodiscard]] virtual data::admin get_business() const;
	virtual void set_subject(const std::string&);
	[[nodiscard]] virtual std::string get_subject() const;

private:
	[[nodiscard]] bool have_file_names(const std::vector<std::string>&);
	void set_flag(const int&);
	void clear_flag(const int&);
	[[nodiscard]] bool check_flags() const;

private:
	using mask_type = std::uint8_t;

	std::vector<std::string> attachments{""};
	data::client client{};
	data::admin admin{};
	std::string subject{""};
	mask_type flags{0x0};
	std::mutex email_data{};
	mask_type mask{0xF};
	enum FLAG {
		PDF = 0,
		CLIENT,
		ADMIN,
		SUBJECT
	};

	enum BIT {
		CLEAR = 0,
		SET
	};
};
}
#endif
