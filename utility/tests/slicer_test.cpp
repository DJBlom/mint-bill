/*******************************************************************************
 * Contents: Generate PDF feature unit tests
 * Author: Dawid Blom
 * Date: December 23, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <slicer.h>
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

static const std::string long_description{"Beneath the sky so vast and blue, Where whispers weave the morning dew, The trees stand tall, their shadows play, A dance of life, a soft ballet. The river hums a timeless tune, Reflecting light from sun to moon, It carries dreams to oceans deep, A secret world where hopes may sleep. The stars ignite the velvet night, Their spark, a beacon’s gentle light. Through every breath, through joy and strife, We live, we dream--the dance of life. Forever flows this fleeting glow. Test completed, good!"};

/**********************************TEST LIST************************************
 * 1) Slice a string of 500 characters into strings of 40 characters. (Done)
 * 2) Ensure that strings are not slicing words. (Done)
 * 3) When a word is sliced it should be resliced to be cut off at the
 *    previous white space. (Done)
 * 4) Ensure thread safety. (Done)
 ******************************************************************************/
TEST_GROUP(slicer_test)
{
        utility::slicer slicer{40};
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(slicer_test, slice_a_500_character_string)
{
        std::vector<std::string> result{slicer.slice(long_description)};

        int i{1};
        for (const auto& str : result)
        {
                STRCMP_EQUAL(expected[i], str.c_str());
                ++i;
        }
}
