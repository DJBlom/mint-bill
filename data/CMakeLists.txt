############################################################################
# Contents: Feature CMakeLists.txt file
# Author: Dawid Blom
# Date: September 15, 2023
#
# NOTE:
############################################################################
cmake_minimum_required(VERSION 3.25)
project(target-data VERSION 0.0.1 LANGUAGES CXX C)


file(GLOB RESULT ${PROJECT_SOURCE_DIR}/source)
list(LENGTH RESULT FILES_IN_DIR)
if(${FILES_IN_DIR} GREATER 0)
        add_library(data 
                OBJECT
                ${PROJECT_SOURCE_DIR}/source/business_data.cpp
                ${PROJECT_SOURCE_DIR}/source/client_data.cpp
        )

        target_include_directories(data 
                PRIVATE
                ${PROJECT_SOURCE_DIR}/include/
        )
else ()
        message(STATUS "There are no files in, data/source.")
endif()
