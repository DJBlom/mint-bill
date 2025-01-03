/*******************************************************************************
 * Contents: Read file feature unit tests
 * Author: Dawid Blom
 * Date: December 30, 2024
 *
 * Note: Refer to the TEST LIST for details on what this fixture tests.
 ******************************************************************************/
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"


#include <file.h>
#include <string>
extern "C"
{

}

static const char* plain_text{"Hello, world\n"
        "\n"
        "This is an email body with an invoice attachment.\n"
        "Please find attached your invoice.\n"
        "\n"
        "Regards,\n"
        "Test Surname\n"
};

static const char* html{
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "    <style>\n"
        "        body {\n"
        "            margin: 0;\n"
        "            padding: 0;\n"
        "            font-family: Arial, sans-serif;\n"
        "            background-color: #f4f4f4;\n"
        "        }\n"
        "        table {\n"
        "            border-spacing: 0;\n"
        "            width: 100%;\n"
        "        }\n"
        "        img {\n"
        "            max-width: 100%;\n"
        "            height: auto;\n"
        "            display: block;\n"
        "        }\n"
        "        .content {\n"
        "            max-width: 600px;\n"
        "            margin: 0 auto;\n"
        "            background-color: #ffffff;\n"
        "            padding: 20px;\n"
        "            border-radius: 5px;\n"
        "            box-shadow: 0 2px 3px rgba(0, 0, 0, 0.1);\n"
        "        }\n"
        "        .header {\n"
        "            background-color: #007bff;\n"
        "            color: #ffffff;\n"
        "            text-align: center;\n"
        "            padding: 20px;\n"
        "            font-size: 24px;\n"
        "        }\n"
        "        .body {\n"
        "            font-size: 16px;\n"
        "            color: #333333;\n"
        "            line-height: 1.6;\n"
        "        }\n"
        "        .footer {\n"
        "            font-size: 12px;\n"
        "            color: #999999;\n"
        "            text-align: center;\n"
        "            margin-top: 20px;\n"
        "        }\n"
        "        .button {\n"
        "            display: inline-block;\n"
        "            padding: 10px 20px;\n"
        "            background-color: #007bff;\n"
        "            color: #ffffff;\n"
        "            text-decoration: none;\n"
        "            border-radius: 5px;\n"
        "            margin: 20px 0;\n"
        "        }\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <table>\n"
        "        <tr>\n"
        "            <td>\n"
        "                <div class=\"content\">\n"
        "                    <div class=\"header\">\n"
        "                        Email Subject or Header\n"
        "                    </div>\n"
        "                    <div class=\"body\">\n"
        "                        <p>Hello,</p>\n"
        "                        <p>This is a sample email message. Use this template to send clean and professional emails.</p>\n"
        "                        <p>You can include <b>HTML elements</b> like paragraphs, lists, or even images:</p>\n"
        "                        <ul>\n"
        "                            <li>Item 1</li>\n"
        "                            <li>Item 2</li>\n"
        "                            <li>Item 3</li>\n"
        "                        </ul>\n"
        "                        <p>\n"
        "                            <img src=\"https://via.placeholder.com/600x200\" alt=\"Sample Image\" />\n"
        "                        </p>\n"
        "                        <p>For more details, click the button below:</p>\n"
        "                        <p><a href=\"https://example.com\" class=\"button\">Learn More</a></p>\n"
        "                        <p>Thank you!</p>\n"
        "                    </div>\n"
        "                    <div class=\"footer\">\n"
        "                        &copy; 2024 Your Company. All rights reserved.\n"
        "                    </div>\n"
        "                </div>\n"
        "            </td>\n"
        "        </tr>\n"
        "    </table>\n"
        "</body>\n"
        "</html>\n"
};


/**********************************TEST LIST************************************
 * 1) Read data from a file based path that should be provided. (Done)
 * 2) Ensure RAII. (Done)
 * 3) Ensure thread safety.
 * 4) Ensure non-copy or move semantics. (Done)
 * 5) Ensure plain text files can be read. (Done)
 * 6) Ensure html files can be read. (Done)
 ******************************************************************************/
TEST_GROUP(file_test)
{
	void setup()
	{
	}

	void teardown()
	{
	}
};

TEST(file_test, ensure_plain_text_files_can_be_read)
{
        utility::file file{"email.txt"};
        std::string result{file.read()};

        STRCMP_EQUAL(plain_text, result.c_str());
}

TEST(file_test, ensure_html_files_can_be_read)
{
        utility::file file{"email.html"};
        std::string result{file.read()};

        STRCMP_EQUAL(html, result.c_str());
}
