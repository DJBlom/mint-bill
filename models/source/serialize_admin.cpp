
#include <admin_data.h>
#include <serialize_admin.h>


utility::serialize_admin::~serialize_admin() {}

storage::database::sql_parameters
utility::serialize_admin::package_data(const std::any& _data)
{
	(void) _data;
	storage::database::sql_parameters params = {};

	return params;
}

std::any
utility::serialize_admin::extract_data(const storage::database::part::rows& _rows)
{
	(void) _rows;
	data::admin admin_data{};

	return admin_data;
}
