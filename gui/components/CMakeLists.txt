############################################################################
# Contents: Graphical User Interface CMakeLists.txt file
# Author: Dawid Blom
# Date: November 13, 2024
#
# NOTE:
############################################################################
cmake_minimum_required(VERSION 3.25)
project(target-components VERSION 0.0.1 LANGUAGES CXX C)


file(GLOB RESULT ${PROJECT_SOURCE_DIR}/source)
list(LENGTH RESULT FILES_IN_DIR)
if(${FILES_IN_DIR} GREATER 0)

        add_library(components
                OBJECT
                ${PROJECT_SOURCE_DIR}/source/gui_parts.cpp
                ${PROJECT_SOURCE_DIR}/source/stack.cpp
        )

        target_include_directories(components
                PRIVATE
                ${GTKMM_INCLUDE_DIRS}
                ${PROJECT_SOURCE_DIR}/include/
                PUBLIC
                ${CMAKE_SOURCE_DIR}/app/include
                ${CMAKE_SOURCE_DIR}/data/include
                ${CMAKE_SOURCE_DIR}/storage/include
                ${CMAKE_SOURCE_DIR}/features/include
                ${CMAKE_SOURCE_DIR}/utility/include
                ${PROJECT_SOURCE_DIR}/invoice_page/include
        )
else ()
        message(STATUS "There are no files in, gui/source.")
endif()
