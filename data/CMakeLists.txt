###############################################################################
# @file CMakeLists.txt
#
# @brief
#   CMake configuration for building the target-data module, which provides
#   the full set of data-model types used across the application.
#
# @details
#   This CMakeLists file defines an OBJECT library named 'data' containing the
#   various source files for business, client, invoice, statement, email, and
#   PDF-related data structures. Before building, it checks whether the source
#   directory contains files and emits a status message if it does not.
#
#   The library exposes public include directories for downstream targets while
#   keeping internal includes private, ensuring clean dependency separation.
#
# @responsibilities
#   - Define the target-data project and build settings.
#   - Build an OBJECT library composed of all data-model source files.
#   - Provide proper include paths for public and private consumers.
#   - Perform a runtime check ensuring the source directory is populated.
#
# @notes
#   The data library is an OBJECT target, meaning it contributes compiled
#   object files to other targets rather than producing a standalone library
#   artifact. Adjust source lists and include paths as new components evolve.
###############################################################################
cmake_minimum_required(VERSION 3.25)
project(target-data VERSION 0.0.1 LANGUAGES CXX C)


file(GLOB RESULT ${PROJECT_SOURCE_DIR}/source)
list(LENGTH RESULT FILES_IN_DIR)
if(${FILES_IN_DIR} GREATER 0)
        add_library(data
                OBJECT
                ${PROJECT_SOURCE_DIR}/source/business_data.cpp
                ${PROJECT_SOURCE_DIR}/source/admin_data.cpp
                ${PROJECT_SOURCE_DIR}/source/email_data.cpp
                ${PROJECT_SOURCE_DIR}/source/client_data.cpp
                ${PROJECT_SOURCE_DIR}/source/column_data.cpp
                ${PROJECT_SOURCE_DIR}/source/billing_data.cpp
                ${PROJECT_SOURCE_DIR}/source/invoice_data.cpp
                ${PROJECT_SOURCE_DIR}/source/statement_data.cpp
                ${PROJECT_SOURCE_DIR}/source/pdf_invoice_data.cpp
                ${PROJECT_SOURCE_DIR}/source/pdf_statement_data.cpp
        )

        target_include_directories(data
                PRIVATE
                ${PROJECT_SOURCE_DIR}/include/
                PUBLIC
                ${CMAKE_SOURCE_DIR}/utility/include
        )
else ()
        message(STATUS "There are no files in, data/source.")
endif()
