/********************************************************
 * Contents: Data interface definition
 * Author: Dawid J. Blom
 * Date: November 16, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _DATA_H_
#define _DATA_H_

namespace interface {
        class data {
                public:
                        virtual ~data() = default;

                        [[nodiscard]] virtual bool is_valid() = 0;
        };
}
#endif
