/********************************************************
 * Contents: File implementation
 * Author: Dawid J. Blom
 * Date: December 30, 2024
 *
 * NOTE:
 *******************************************************/
#include <file.h>


utility::file::file(const std::string& _path)
        : current_file{std::move(std::filesystem::absolute(_path)), std::ios_base::in}
{
        if (!this->current_file.is_open())
                throw app::errors::construction;
}

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
