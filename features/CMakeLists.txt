############################################################################
# Contents: Feature CMakeLists.txt file
# Author: Dawid Blom
# Date: September 15, 2023
#
# NOTE:
# This file is the CMakeLists.txt for the feature sub branch file, containing
# the source and header files of the feautre sub branch.
############################################################################
cmake_minimum_required(VERSION 3.25)
project(target-features VERSION 0.0.1 LANGUAGES CXX C)


file(GLOB RESULT ${PROJECT_SOURCE_DIR}/source)
list(LENGTH RESULT FILES_IN_DIR)
if(${FILES_IN_DIR} GREATER 0)
        find_package(CURL REQUIRED)
        add_library(features 
                OBJECT
                ${PROJECT_SOURCE_DIR}/source/business_info.cpp
                ${PROJECT_SOURCE_DIR}/source/client_register.cpp
                ${PROJECT_SOURCE_DIR}/source/client_invoice.cpp
                ${PROJECT_SOURCE_DIR}/source/email.cpp
        )

        target_include_directories(features 
                PRIVATE
                ${PROJECT_SOURCE_DIR}/include/
                ${CURL_INCLUDE_DIR}
                PUBLIC
                ${CMAKE_SOURCE_DIR}/data/include
                ${CMAKE_SOURCE_DIR}/storage/include
                ${CMAKE_SOURCE_DIR}/utility/include
        )
else ()
        message(STATUS "There are no files in, features/source.")
endif()
