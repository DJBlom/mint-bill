/*******************************************************************************
 * @file    file.cpp
 * @brief   Implementation of the thread-safe file reader utility.
 *
 * @details Defines the logic for opening, validating, reading, and closing a
 *          filesystem resource. The constructor resolves the supplied path to an
 *          absolute location before attempting to open the file, and throws an
 *          application-level construction error on failure.
 *
 *          read()
 *            • Locks an internal mutex to ensure exclusive read access.
 *            • Streams the entire file into a string buffer.
 *            • Returns the result as a std::string.
 *
 *          The destructor closes the underlying file stream, maintaining proper
 *          RAII semantics and preventing resource leaks.
 ******************************************************************************/
#include <file.h>


utility::file::file(const std::string& _path)
        : current_file{std::move(std::filesystem::absolute(_path)), std::ios_base::in}
{
        if (!this->current_file.is_open()) //GCOVR_EXCL_START
                throw app::errors::construction;
} //GCOVR_EXCL_STOP

utility::file::~file()
{
        this->current_file.close();
}

std::string utility::file::read()
{
        std::ostringstream data{""};
        std::lock_guard<std::mutex> guard(this->file_mutex);
        data << this->current_file.rdbuf();

        return data.str();
}
