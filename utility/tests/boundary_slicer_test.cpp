/*******************************************************************************
 * Contents: String slicer feature unit tests
 * Author: Dawid Blom
 * Date: December 23, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <boundary_slicer.h>
extern "C"
{

}

static const char* expected[510] = {"",
        "Beneath the sky so vast and blue, Where",
        "whispers weave the morning dew, The",
        "trees stand tall, their shadows play, A",
        "dance of life, a soft ballet. The river",
        "hums a timeless tune, Reflecting light",
        "from sun to moon, It carries dreams to",
        "oceans deep, A secret world where hopes",
        "may sleep. The stars ignite the velvet",
        "night, Their spark, a beacon’s gentle",
        "light. Through every breath, through joy",
        "and strife, We live, we dream--the dance",
        "of life. Forever flows this fleeting",
        "glow. Test completed, good!"};

static const char* expected_bad[510] = {"",
        "Beneath the sky so vast and blue, Where",
        "whispers weave the morning dew, The",
        "trees stand tall, their shadows play, A",
        "dance of life, a soft ballet. The river",
        "hums a timeless tune, Reflecting light",
        "from sun to moon, It carries dreams to",
        "oceans deep, A secret world where hopes",
        "may sleep. The stars ignite the velvet",
        "night, Their spark, a beacon’s gentle",
        "light. Through every breath, through joy",
        "and strife, We live, we dream--the dance",
        "of life. Forever flows this fleeting",
        ""};
static const std::string long_description{"Beneath the sky so vast and blue, Where whispers weave the morning dew, The trees stand tall, their shadows play, A dance of life, a soft ballet. The river hums a timeless tune, Reflecting light from sun to moon, It carries dreams to oceans deep, A secret world where hopes may sleep. The stars ignite the velvet night, Their spark, a beacon’s gentle light. Through every breath, through joy and strife, We live, we dream--the dance of life. Forever flows this fleeting glow. Test completed, good!"};

static const std::string bad_description{"Beneath the sky so vast and blue, Where whispers weave the morning dew, The trees stand tall, their shadows play, A dance of life, a soft ballet. The river hums a timeless tune, Reflecting light from sun to moon, It carries dreams to oceans deep, A secret world where hopes may sleep. The stars ignite the velvet night, Their spark, a beacon’s gentle light. Through every breath, through joy and strife, We live, we dream--the dance of life. Forever flows this fleeting                            "};

/**********************************TEST LIST************************************
 * 1) Slice a string of 500 characters into strings of 40 characters. (Done)
 * 2) Ensure that strings are not slicing words. (Done)
 * 3) When a word is sliced it should be resliced to be cut off at the
 *    previous white space. (Done)
 * 4) Ensure thread safety. (Done)
 ******************************************************************************/
TEST_GROUP(boundary_slicer_test)
{
        utility::boundary_slicer slicer;
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(boundary_slicer_test, test_max_adjusting)
{
        utility::boundary_slicer tmp_slicer{500};
}

TEST(boundary_slicer_test, slice_a_500_character_string)
{
        std::vector<std::string> result{slicer.slice(long_description)};

        int i{1};
        for (const auto& str : result)
        {
                STRCMP_EQUAL(expected[i], str.c_str());
                ++i;
        }
}

TEST(boundary_slicer_test, slice_empty_data)
{
        std::vector<std::string> result{slicer.slice("")};

        int i{0};
        for (const auto& str : result)
        {
                STRCMP_EQUAL(expected[i], str.c_str());
                ++i;
        }
}

TEST(boundary_slicer_test, empty_last_line_in_string)
{
        std::vector<std::string> result{slicer.slice(bad_description)};

        int i{1};
        for (const auto& str : result)
        {
                STRCMP_EQUAL(expected_bad[i], str.c_str());
                ++i;
        }
}

TEST(boundary_slicer_test, check_the_boundary_for_400_plus_word_sizes)
{
        std::vector<std::string> result{slicer.slice("ddddddddddddddddddddddddddddddddddddddd\
                                                      ddddddddddddddddddddddddddddddddddddddd\
                                                      ddddddddddddddddddddddddddddddddddddddd\
                                                      ddddddddddddddddddddddddddddddddddddddd")};

        for (const auto& str : result)
        {
                STRCMP_EQUAL("ddddddddddddddddddddddddddddddddddddddd", str.c_str());
        }
}
