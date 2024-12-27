/********************************************************
 * Contents: Storage class implementation
 * Author: Dawid J. Blom
 * Date: November 25, 2024
 *
 * NOTE:
 *******************************************************/
#include <sql.h>

storage::sql::~sql()
{

}

bool storage::sql::is_open() const
{
        return true;
}
