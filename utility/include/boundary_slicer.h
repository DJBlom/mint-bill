/********************************************************
 * Contents: Slicer definition
 * Author: Dawid J. Blom
 * Date: December 28, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _BOUNDARY_SLICER_H_
#define _BOUNDARY_SLICER_H_
#include <mutex>
#include <string>
#include <vector>
#include <sstream>
#include <slicer.h>

namespace utility {
        class boundary_slicer : public interface::slicer {
                public:
                        boundary_slicer() = default;
                        explicit boundary_slicer(const std::string::size_type& _max) : max{_max} {}
                        boundary_slicer(const boundary_slicer&) = delete;
                        boundary_slicer(boundary_slicer&&) = delete;
                        boundary_slicer& operator= (const boundary_slicer&) = delete;
                        boundary_slicer& operator= (boundary_slicer&&) = delete;
                        virtual ~boundary_slicer() override;

                        [[nodiscard]] virtual std::vector<std::string> slice(const std::string&) override;

                private:
                        [[nodiscard]] bool word_does_not_exceed_max();
                        [[nodiscard]] bool word_does_exceed_max();
                        void add_word_to_current_line();
                        void handle_large_word(std::vector<std::string>&);
                        void add_whats_left_over(std::vector<std::string>&);
                        void add_new_current_line(std::vector<std::string>&);

                private:
                        std::string word{""};
                        std::mutex data_mutex{};
                        std::string current_line{""};
                        std::string::size_type max{40};

                        enum DECISION {
                                NO = 0,
                                YES
                        };
        };
}
#endif
