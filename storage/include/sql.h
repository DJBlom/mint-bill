/********************************************************
 * Contents: SQL class definition
 * Author: Dawid J. Blom
 * Date: November 25, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _SQL_H_
#define _SQL_H_
#include <storage.h>

namespace storage {
        class sql : public interface::storage {
                public:
                        virtual ~sql() override;

                        [[nodiscard]] virtual bool is_open() const override;
        };
}
#endif
