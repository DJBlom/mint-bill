/********************************************************
 * Contents: Storage interface definition
 * Author: Dawid J. Blom
 * Date: November 25, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _STORAGE_H_
#define _STORAGE_H_

namespace interface {
class storage {
public:
	virtual ~storage() = default;

	// [[nodiscard]] virtual bool insert() const = 0;
	// [[nodiscard]] virtual bool retrieve() const = 0;
};
}
#endif
