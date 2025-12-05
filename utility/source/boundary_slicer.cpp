/*******************************************************************************
 * @file    boundary_slicer.cpp
 * @brief   Implementation of the boundary-based text slicing utility.
 *
 * @details This file provides the runtime logic for converting a long string
 *          into multiple bounded lines. The implementation follows a structured
 *          decision model:
 *
 *            - Determine whether the next word fits on the current line.
 *            - Slice oversized words into multiple fixed-width segments.
 *            - Emit completed lines into the output vector before starting new
 *              ones.
 *            - Append any leftover buffered text at the end.
 *
 *          Internal helpers encapsulate each decision:
 *            • word_does_not_exceed_max() — safe to append word to line.
 *            • word_does_exceed_max()     — word must be segmented.
 *            • add_word_to_current_line() — appends with optional whitespace.
 *            • add_new_current_line()     — flushes current line and begins new.
 *            • handle_large_word()        — slices very long words.
 *            • add_whats_left_over()      — emits the trailing line.
 *
 *          The design ensures deterministic line wrapping suitable for
 *          UI rendering, PDF creation, or any text environment requiring
 *          predictable formatting boundaries.
 ******************************************************************************/
#include <boundary_slicer.h>


utility::boundary_slicer::~boundary_slicer() {}

std::vector<std::string> utility::boundary_slicer::slice(const std::string& _data)
{
        this->sliced_data.clear();
        this->current_line.clear();
        if (!_data.empty())
        {
		std::string word{};
                std::istringstream stream{_data};
                while (stream >> word)
                {
                        if (word_does_not_exceed_max(word))
                        {
                                add_word_to_current_line(word);
                        }
                        else if (word_does_exceed_max(word))
                        {
                                handle_large_word(word);
                        }
                        else
                        {
                                add_new_current_line(word);
                        }
                }
                add_whats_left_over();
        }

        return this->sliced_data;
}

bool utility::boundary_slicer::word_does_not_exceed_max(const std::string& _word)
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        return (this->current_line.size() +
                        _word.size() +
                        (this->current_line.empty() ?
                         DECISION::NO : DECISION::YES) <= this->max);
}

bool utility::boundary_slicer::word_does_exceed_max(const std::string& _word)
{
        return (_word.size() >= this->max);
}

void utility::boundary_slicer::add_word_to_current_line(const std::string& _word)
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        if (!this->current_line.empty())
        {
                this->current_line += " ";
        }

        this->current_line += _word;
}

void utility::boundary_slicer::handle_large_word(const std::string& _word)
{
        size_t pos{0};
        while (pos < _word.size())
        {
                size_t len = std::min(this->max, _word.size() - pos);
                this->sliced_data.emplace_back(_word.substr(pos, len));
                pos += len;
        }
}

void utility::boundary_slicer::add_new_current_line(const std::string& _word)
{
        std::lock_guard<std::mutex> guard(this->data_mutex);
        this->sliced_data.push_back(this->current_line);
        this->current_line = _word;
}

void utility::boundary_slicer::add_whats_left_over()
{
        this->sliced_data.push_back(this->current_line);
}
