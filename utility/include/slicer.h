/********************************************************
 * Contents: Slicer definition
 * Author: Dawid J. Blom
 * Date: December 28, 2024
 *
 * NOTE:
 *******************************************************/
#ifndef _SLICER_H_
#define _SLICER_H_
#include <mutex>
#include <string>
#include <vector>

namespace utility {
        class slicer {
                public:
                        slicer() = default;
                        explicit slicer(const std::string::size_type& _max) : max{_max} {}
                        slicer(const slicer&) = delete;
                        slicer(slicer&&) = delete;
                        slicer& operator= (const slicer&) = delete;
                        slicer& operator= (slicer&&) = delete;
                        virtual ~slicer() = default;

                        [[nodiscard]] virtual std::vector<std::string> slice(const std::string&);

                private:
                        [[nodiscard]] bool word_does_not_exceed_max();
                        //void add_full_lines(const std::string&);
                        void add_word_to_current_line();
                        void add_new_current_line(std::vector<std::string>&);
                        void add_whats_left_over(std::vector<std::string>&);

                private:
                        std::string::size_type max{40};
                        std::string word{""};
                        std::string current_line{""};
                        std::mutex data_mutex{};

                        enum DECISION {
                                NO = 0,
                                YES
                        };
        };
}
#endif
