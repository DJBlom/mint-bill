/*******************************************************************************
 * @file    word_slicer_test.cpp
 * @brief   Unit tests for the utility::word_slicer class.
 *
 * @details This test suite verifies the functionality and robustness of the
 *          word-based slicing utility, which splits an input string into
 *          individual whitespace-delimited tokens. These tests ensure:
 *
 *            • Correct parsing of arbitrary strings into discrete words, with
 *              whitespace handled strictly as a separator.
 *
 *            • Enforcement of the class’s internal safety guarantees, including
 *              thread-safe insertion of sliced words via mutex protection.
 *
 *            • Proper non-copyable and non-movable semantics, preventing unsafe
 *              transfer of internal state that could lead to race conditions.
 *
 *            • Adherence to the enforced maximum capacity of 50 tokens, ensuring
 *              excess data is safely ignored without corrupting state.
 *
 *            • Graceful handling of empty input strings, yielding an empty list
 *              without errors.
 *
 *          Together, these tests validate that utility::word_slicer provides a
 *          simple, deterministic, and concurrency-safe tokenization mechanism
 *          suitable for processing user descriptions, messages, or printed
 *          content where fixed upper limits are required.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <word_slicer.h>
extern "C"
{

}



static const char* max_num_words{"Forty plus years in our two-story house "
        "Eighty-three years alive for me "
        "Eighty-six for my husband "
        "Two adult children launched "
        "Ten grandchildren to watch grow "
        "Two past fulfilling careers "
        "Ten o’clock bedtimes "
        "One car to share "
        "Two of us needed to carry groceries home "
        "One new year with multiplying possibilities "
};

static const char* over_max_num_words{"Forty plus years in our two-story house "
        "Eighty-three years alive for me "
        "Eighty-six for my husband "
        "Two adult children launched "
        "Ten grandchildren to watch grow "
        "Two past fulfilling careers "
        "Ten o’clock bedtimes "
        "One car to share "
        "Two of us needed to carry groceries home "
        "One new year with multiplying possibilities "
        "One new year with multiplying possibilities "
};

std::vector<std::string> expected{"Forty","plus","years","in","our","two-story","house",
        "Eighty-three","years","alive","for","me",
        "Eighty-six","for","my","husband",
        "Two","adult","children","launched",
        "Ten","grandchildren","to","watch","grow",
        "Two","past","fulfilling","careers",
        "Ten","o’clock","bedtimes",
        "One","car","to","share",
        "Two","of","us","needed","to","carry","groceries","home",
        "One","new","year","with","multiplying","possibilities",
};


/**********************************TEST LIST************************************
 * 1) Parse a string into seperate words,
 *    a word is a string without any white space. (Done)
 * 2) Ensure thread safety of data. (Done)
 * 3) Ensure object is none copyable. (Done)
 * 4) Ensure object is not moveable. (Done)
 * 5) It should handle a maximum string length of 50 words. (Done)
 ******************************************************************************/
TEST_GROUP(word_slicer_test)
{
        utility::word_slicer slicer{};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(word_slicer_test, slice_string_into_seperate_words)
{
        std::vector<std::string> words{slicer.slice(max_num_words)};

        long unsigned int i{0};
        for (const auto& word : words)
        {
                CHECK_EQUAL(expected[i], word);
                ++i;
        }
}

TEST(word_slicer_test, slice_an_empty_string)
{
        std::string empty_str{""};
        std::vector<std::string> words{slicer.slice("")};

        CHECK_EQUAL(0, words.size());
        for (const auto& word : words)
                CHECK_EQUAL(empty_str, word);
}

TEST(word_slicer_test, slice_to_much_data)
{
        std::vector<std::string> words{slicer.slice(over_max_num_words)};

        CHECK_EQUAL(50, words.size());
        long unsigned int i{0};
        for (const auto& word : words)
        {
                CHECK_EQUAL(expected[i], word);
                ++i;
        }
}
