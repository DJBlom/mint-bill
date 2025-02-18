/********************************************************
 * Contents: Word slicer implementation
 * Author: Dawid J. Blom
 * Date: December 28, 2024
 *
 * NOTE:
 *******************************************************/
#include <word_slicer.h>

namespace limit {
        constexpr long unsigned int max{50};
}

utility::word_slicer::~word_slicer() {}

std::vector<std::string> utility::word_slicer::slice(const std::string& _data)
{
        this->sliced_data.clear();
        if (data_ok(_data) == true)
        {
                slice_data(_data);
        }

        return this->sliced_data;
}

bool utility::word_slicer::data_ok(const std::string& _data)
{
        return (!_data.empty());
}

bool utility::word_slicer::not_maxed()
{
        return (this->sliced_data.size() < limit::max);
}

void utility::word_slicer::slice_data(const std::string& _data)
{
        std::string word{""};
        std::istringstream stream{_data};
        while ((stream >> word) && not_maxed())
        {
                safely_add(word);
        }
}

void utility::word_slicer::safely_add(const std::string& _word)
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        this->sliced_data.push_back(_word);
}
