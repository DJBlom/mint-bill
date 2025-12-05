/*******************************************************************************
 * @file    boundary_slicer.h
 * @brief   Line-boundary text slicer with configurable maximum width.
 *
 * @details This class implements a deterministic, thread-safe text slicing
 *          utility that breaks an input string into multiple lines according to
 *          a caller-specified maximum character boundary.
 *
 *          The algorithm processes text word-by-word and applies the following
 *          behavior:
 *
 *            - Words that fit on the current line are appended normally.
 *            - Words that exceed the maximum length are sliced into multiple
 *              segments of size @ref max.
 *            - Words that would overflow the current line cause a new line to
 *              be started.
 *
 *          Concurrency:
 *            A mutex protects updates to the mutable internal buffer
 *            (`current_line`) ensuring safe usage in multi-threaded contexts.
 *
 *          This class derives from interface::slicer, providing a concrete
 *          implementation of the slice() operation for use by formatting,
 *          printing, and PDF-generation components.
 ******************************************************************************/
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
                        [[nodiscard]] bool word_does_not_exceed_max(const std::string&);
                        [[nodiscard]] bool word_does_exceed_max(const std::string&);
                        void add_word_to_current_line(const std::string&);
                        void handle_large_word(const std::string&);
                        void add_new_current_line(const std::string&);
                        void add_whats_left_over();

                private:
                        std::mutex data_mutex{};
                        std::string current_line{""};
                        std::string::size_type max{40};
                        std::vector<std::string> sliced_data{};

                        enum DECISION {
                                NO = 0,
                                YES
                        };
        };
}
#endif
