/*******************************************************************************
 * @file    word_slicer.h
 * @brief   Word-by-word slicing utility.
 *
 * @details Defines a simple slicer implementation that splits an input string
 *          into individual whitespace-delimited words. The class implements the
 *          shared slicer interface and enforces basic constraints such as:
 *
 *            • Rejecting empty input strings.
 *            • Limiting the maximum number of output words.
 *            • Thread-safe modification of the internal sliced_data container.
 *
 *          Typical use cases include preparing text for UI display, formatting
 *          invoice content, or breaking larger strings into manageable chunks.
 ******************************************************************************/
#ifndef _WORD_SLICER_H_
#define _WORD_SLICER_H_
#include <mutex>
#include <string>
#include <vector>
#include <sstream>
#include <slicer.h>

namespace utility {
        class word_slicer : public interface::slicer {
                public:
                        word_slicer() = default;
                        word_slicer(const word_slicer&) = delete;
                        word_slicer(word_slicer&&) = delete;
                        word_slicer& operator= (const word_slicer&) = delete;
                        word_slicer& operator= (word_slicer&&) = delete;
                        virtual ~word_slicer() override;

                        [[nodiscard]] virtual std::vector<std::string> slice(const std::string&) override;

                private:
                        [[nodiscard]] bool data_ok(const std::string&);
                        [[nodiscard]] bool not_maxed();
                        void slice_data(const std::string&);
                        void safely_add(const std::string&);

                private:
                        std::mutex data_mutex{};
                        std::vector<std::string> sliced_data{""};
        };
}
#endif
