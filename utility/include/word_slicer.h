/********************************************************
 * Contents: Slicer definition
 * Author: Dawid J. Blom
 * Date: December 28, 2024
 *
 * NOTE:
 *******************************************************/
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
