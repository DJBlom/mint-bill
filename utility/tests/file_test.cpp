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
"<style>\n"
"body {\n"
"margin: 0;\n"
"padding: 0;\n"
"font-family: monospace;\n"
"background-color: #3d3d5c;\n"
"}\n"
"table {\n"
"border-spacing: 0;\n"
"width: 100%;\n"
"}\n"
"img {\n"
"max-width: 100%;\n"
"height: auto;\n"
"display: block;\n"
"}\n"
".content {\n"
"max-width: 600px;\n"
"margin: 0 auto;\n"
"background-color: #DAE1ED;\n"
"padding: 20px;\n"
"border-radius: 5px;\n"
"box-shadow: 5px 5px 5px 5px rgba(0, 0, 0, 0.8);\n"
"}\n"
".header {\n"
"background-color: #4C679A;\n"
"color: #2A3955;\n"
"text-align: center;\n"
"padding: 20px;\n"
"font-size: 24px;\n"
"font-weight: bold;\n"
"border-radius: 5px;\n"
"}\n"
".body {\n"
"font-size: 16px;\n"
"color: #151B26;\n"
"line-height: 1.6;\n"
"}\n"
".footer {\n"
"font-size: 12px;\n"
"background-color: #2A3955;\n"
"color: #DAE1ED;\n"
"text-align: center;\n"
"margin-top: 20px;\n"
"padding: 10px;\n"
"border-radius: 5px;\n"
"}\n"
"</style>\n"
"</head>\n"
"<body>\n"
"<table>\n"
"<tr>\n"
"<td>\n"
"<div class=\"content\">\n"
"<div class=\"header\">\n"
"Greatings, {{CLIENT_NAME}}!\n"
"</div>\n"
"<div class=\"body\">\n"
"<p>Please find attached your {{CLIENT_DOCUMENT}}.</p>\n"
"<p><b>Contact:</b></p>\n"
"<ul>\n"
"<li>Cell: {{BUSINESS_CELLPHONE}} </li>\n"
"<li>Email: {{BUSINESS_EMAIL}}</li>\n"
"</ul>\n"
"<p>Thank you!</p>\n"
"</div>\n"
"<div class=\"footer\">\n"
"&copy; {{BUSINESS_NAME}}. All rights reserved.\n"
"</div>\n"
"</div>\n"
"</td>\n"
"</tr>\n"
"</table>\n"
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
