/*******************************************************************************
 * @file    slicer.h
 * @brief   Abstract interface for string-slicing utilities.
 *
 * @details This header defines the base slicer interface used throughout the
 *          system to transform input strings into collections of substrings.
 *          Concrete implementations (e.g., boundary_slicer, word_slicer) are
 *          responsible for providing specific slicing strategies such as:
 *
 *            • Fixed-width boundary slicing.
 *            • Word-by-word tokenization.
 *            • Hybrid or custom segmentation policies.
 *
 *          The interface enforces:
 *            • A uniform slice(...) method returning a vector of strings.
 *            • No assumptions about formatting or parsing rules.
 *            • Polymorphic destruction through a virtual destructor.
 *
 *          This abstraction allows interchangeable slicing behavior across UI,
 *          formatting, and storage components.
 ******************************************************************************/
#ifndef _SLICER_H_
#define _SLICER_H_
#include <mutex>
#include <string>
#include <vector>
#include <sstream>

namespace interface {
        class slicer {
                public:
                        virtual ~slicer() = default;

                        [[nodiscard]] virtual std::vector<std::string> slice(const std::string&) = 0;
        };
}
#endif
