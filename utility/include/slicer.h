/********************************************************
 * Contents: Slicer definition
 * Author: Dawid J. Blom
 * Date: December 28, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _SLICER_H_
#define _SLICER_H_
#include <mutex>
#include <string>
#include <vector>
#include <sstream>

namespace interface {
        class slicer {
                public:
                        virtual ~slicer() = default;

                        [[nodiscard]] virtual std::vector<std::string> slice(const std::string&) = 0;
        };
}
#endif
