/********************************************************
 * Contents: Slicer implementation
 * Author: Dawid J. Blom
 * Date: December 28, 2024
 *
 * NOTE:
 *******************************************************/
#include <slicer.h>


std::vector<std::string> utility::slicer::slice(const std::string& _data)
{
        std::vector<std::string> sliced_data{};
        this->word = "";
        this->current_line = "";
        if (!_data.empty())
        {
                std::istringstream stream{_data};
                while (stream >> this->word)
                {
                        if (word_does_not_exceed_max())
                        {
                                add_word_to_current_line();
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

void utility::slicer::add_word_to_current_line()
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        if (!this->current_line.empty())
        {
                current_line += " ";
        }

        this->current_line += this->word;
}

bool utility::slicer::word_does_not_exceed_max()
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        return (this->current_line.size() +
                        this->word.size() +
                        (this->current_line.empty() ?
                         DECISION::NO : DECISION::YES) <= this->max);
}

void utility::slicer::add_whats_left_over(std::vector<std::string>& _data)
{
        _data.push_back(this->current_line);
}

void utility::slicer::add_new_current_line(std::vector<std::string>& _data)
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        _data.push_back(this->current_line);
        this->current_line = this->word;
}
