#include <password_manager.h>

feature::password_manager::password_manager(const std::string& _name): schema_name{_name}
{
	if (this->schema_name.empty() == true)
	{
		throw app::errors::construction;
	}
	else
	{
		this->schema.name = this->schema_name.c_str();
		this->schema.flags = SECRET_SCHEMA_NONE;
		this->schema.attributes[0] = {"number", SECRET_SCHEMA_ATTRIBUTE_INTEGER};
		this->schema.attributes[1] = {"even", SECRET_SCHEMA_ATTRIBUTE_BOOLEAN };
		this->schema.attributes[2] = {nullptr, static_cast<SecretSchemaAttributeType> (0)};
	}
}

feature::password_manager::~password_manager(){}

bool feature::password_manager::store_password(const std::string& _password, const int& _number)
{
	bool success{false};
	if ((_password.empty() == true) || (_number >= std::numeric_limits<int>::max()) || (_number <= std::numeric_limits<int>::min()))
	{
		syslog(LOG_CRIT, "PASSWORD_MANAGER: _password empty or int overflow - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		GError* error{nullptr};
		success = static_cast<bool> (secret_password_store_sync(&this->schema, SECRET_COLLECTION_DEFAULT, "database",
								        _password.c_str(), nullptr, &error,
								        "number", _number,
								        "even", (_number % 2 == 0),
								        nullptr));
		if (error != nullptr) // GCOVR_EXCL_START
		{
			syslog(LOG_CRIT, "PASSWORD_MANAGER: failed to store password - "
					"filename %s, line number %d", __FILE__, __LINE__);
			g_error_free(error);
		}// GCOVR_EXCL_STOP
	}

	return success;
}

std::string feature::password_manager::lookup_password(const int& _number)
{
	std::string password{""};
	if ((_number >= std::numeric_limits<int>::max()) || (_number <= std::numeric_limits<int>::min()))
	{
		syslog(LOG_CRIT, "PASSWORD_MANAGER: int overflow - "
				"filename %s, line number %d", __FILE__, __LINE__);
	}
	else
	{
		GError* error{nullptr};
		gchar* secret = secret_password_lookup_sync(&this->schema, nullptr, &error,
					"number", _number,
					"even", (_number % 2 == 0),
					nullptr);
		if (error != nullptr) // GCOVR_EXCL_START
		{
			syslog(LOG_CRIT, "PASSWORD_MANAGER: failed to look up the password - "
					"filename %s, line number %d", __FILE__, __LINE__);
			g_error_free(error);
		} // GCOVR_EXCL_STOP
		else if (secret == nullptr)
		{
			syslog(LOG_CRIT, "PASSWORD_MANAGER: secret is null - "
					"filename %s, line number %d", __FILE__, __LINE__);
		}
		else
		{
			password = secret;
			secret_password_free(secret);
		}
	}

	return password;
} // GCOVR_EXCL_LINE
