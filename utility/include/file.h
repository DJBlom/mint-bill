/********************************************************
 * Contents: File definition
 * Author: Dawid J. Blom
 * Date: December 30, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _FILE_H_
#define _FILE_H_
#include <mutex>
#include <string>
#include <fstream>
#include <sstream>
#include <errors.h>
#include <filesystem>

namespace utility {
        class file {
                public:
                        file() = delete;
                        explicit file(const std::string&);
                        file(const file&) = delete;
                        file(file&&) = delete;
                        file& operator= (const file&) = delete;
                        file& operator= (file&&) = delete;
                        virtual ~file();

                        [[nodiscard]] virtual std::string read();

                private:
                        std::fstream current_file;
                        std::mutex file_mutex{};
        };
}
#endif
