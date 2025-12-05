/*****************************************************************************
 * @file business_data.h
 *
 * @brief
 *   Declares the core business data model representing contact and identity
 *   information for a business entity.
 *
 * @details
 *   The data::business structure encapsulates common business attributes such
 *   as name, address, area code, town, cellphone number, and one or more
 *   email addresses. It provides a virtual interface for setting and
 *   retrieving these fields, allowing derived types to extend or override
 *   behavior where necessary.
 *
 *   Internal completeness is tracked using a bitmask-based flag mechanism,
 *   which enables a concise is_valid() implementation to verify that all
 *   required attributes are present and well-formed. A mutex is used to
 *   synchronize access to mutable fields, supporting safe use in concurrent
 *   contexts.
 *
 * @responsibilities
 *   Encapsulate core business identity and contact information.
 *   Provide virtual setters and getters for each business attribute.
 *   Validate email formatting via a helper routine.
 *   Track initialization state using an internal flag mask.
 *   Support thread-safe updates using a mutex for synchronization.
 *
 * @notes
 *   This abstraction is intended as a foundation for higher-level data models
 *   (such as administrative or billing records) that require consistent access
 *   to business-level contact and identity information.
 *****************************************************************************/
#ifndef _BUSINESS_DATA_H_
#define _BUSINESS_DATA_H_
#include <mutex>
#include <regex>
#include <string>
#include <cstdint>
#include <word_slicer.h>

namespace data {
struct business {
public:
	business() = default;
	business(const business& new_data);
	business(business&&);
	business& operator= (const business&);
	business& operator= (business&&);
	virtual ~business() = default;

	[[nodiscard]] virtual bool is_valid() const;
	virtual void set_name(const std::string&);
	[[nodiscard]] virtual std::string get_name() const;
	virtual void set_address(const std::string&);
	[[nodiscard]] virtual std::string get_address() const;
	virtual void set_area_code(const std::string&);
	[[nodiscard]] virtual std::string get_area_code() const;
	virtual void set_town(const std::string&);
	[[nodiscard]] virtual std::string get_town() const;
	virtual void set_cellphone(const std::string&);
	[[nodiscard]] virtual std::string get_cellphone() const;
	virtual void set_email(const std::string&);
	[[nodiscard]] virtual std::string get_email() const;

private:
	void set_flag(const int&);
	void clear_flag(const int&);
	[[nodiscard]] bool check_flags() const;
	[[nodiscard]] bool email_address_good(const std::vector<std::string>&);

private:
	using mask_type = std::uint8_t;

	std::string name{""};
	std::string address{""};
	std::string area_code{""};
	std::string town{""};
	std::string cellphone{""};
	std::string email_addresses{""};
	mask_type flags{0x0};
	std::mutex business_data{};
	mask_type mask{0x3F};
	enum FLAG {
		NAME = 0,
		ADDRESS,
		AREA_CODE,
		TOWN,
		CELLPHONE,
		EMAIL
	};

	enum BIT {
		CLEAR = 0,
		SET
	};
};
}
#endif
