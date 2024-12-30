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
project(target-utitlity VERSION 0.0.1 LANGUAGES CXX C)


file(GLOB RESULT ${PROJECT_SOURCE_DIR}/source)
list(LENGTH RESULT FILES_IN_DIR)
if(${FILES_IN_DIR} GREATER 0)
        add_library(utility 
                OBJECT
                ${PROJECT_SOURCE_DIR}/source/slicer.cpp
        )

        target_include_directories(utility 
                PRIVATE
                ${PROJECT_SOURCE_DIR}/include/
        )
else ()
        message(STATUS "There are no files in, utility/source.")
endif()
