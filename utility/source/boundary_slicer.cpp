/********************************************************
 * Contents: Boundary slicer implementation
 * Author: Dawid J. Blom
 * Date: December 28, 2024
 *
 * NOTE:
 *******************************************************/
#include <syslog.h>
#include <boundary_slicer.h>


utility::boundary_slicer::~boundary_slicer() {}

std::vector<std::string> utility::boundary_slicer::slice(const std::string& _data)
{
        std::vector<std::string> sliced_data{};
        this->word.clear();
        this->current_line.clear();
        if (!_data.empty())
        {
                std::istringstream stream{_data};
                while (stream >> this->word)
                {
                        if (word_does_not_exceed_max())
                        {
                                add_word_to_current_line();
                        }
                        else if (word_does_exceed_max())
                        {
                                handle_large_word(sliced_data);
                        }
                        else
                        {
                                add_new_current_line(sliced_data);
                        }
                }
                add_whats_left_over(sliced_data);
        }

        return sliced_data;
} //GCOVR_EXCL_LINE

bool utility::boundary_slicer::word_does_not_exceed_max()
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        return (this->current_line.size() +
                        this->word.size() +
                        (this->current_line.empty() ?
                         DECISION::NO : DECISION::YES) <= this->max);
}

bool utility::boundary_slicer::word_does_exceed_max()
{
        return (this->word.size() >= this->max);
}

void utility::boundary_slicer::add_word_to_current_line()
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        if (!this->current_line.empty())
        {
                this->current_line += " ";
        }

        this->current_line += this->word;
}

void utility::boundary_slicer::handle_large_word(std::vector<std::string>& _data)
{
        size_t pos{0};
        while (pos < this->word.size()) 
        {
                size_t len = std::min(this->max, this->word.size() - pos);
                _data.emplace_back(this->word.substr(pos, len));
                pos += len;
        }
}

void utility::boundary_slicer::add_new_current_line(std::vector<std::string>& _data)
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        _data.push_back(this->current_line);
        this->current_line = this->word;
}

void utility::boundary_slicer::add_whats_left_over(std::vector<std::string>& _data)
{
        _data.push_back(this->current_line);
}
