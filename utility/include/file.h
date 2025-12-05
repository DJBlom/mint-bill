/*******************************************************************************
 * @file    file.h
 * @brief   Thread-safe file reader utility.
 *
 * @details This component provides a minimal RAII wrapper around std::fstream
 *          that ensures safe, exclusive access when reading files. It supports
 *          controlled file opening, mutex-protected read operations, and clean
 *          resource management on destruction.
 *
 *          Features:
 *            • file(path)
 *                Opens a file at the resolved absolute path. Throws a typed
 *                construction error if the file cannot be opened.
 *
 *            • read()
 *                Returns the entire file contents as a string, using a mutex to
 *                guarantee thread-safe access and prevent interleaved reads.
 *
 *          This class is commonly used to load template files, static assets,
 *          or configuration data within the billing and PDF-generation system.
 ******************************************************************************/
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
