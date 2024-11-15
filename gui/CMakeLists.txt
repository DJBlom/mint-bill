############################################################################
# Contents: Graphical User Interface CMakeLists.txt file
# Author: Dawid Blom
# Date: November 13, 2024
#
# NOTE:
############################################################################
cmake_minimum_required(VERSION 3.25)
project(target-gui VERSION 0.0.1 LANGUAGES CXX C)


file(GLOB RESULT ${PROJECT_SOURCE_DIR}/source)
list(LENGTH RESULT FILES_IN_DIR)
if(${FILES_IN_DIR} GREATER 0)

        add_library(gui
                OBJECT
                ${PROJECT_SOURCE_DIR}/source/main_window.cpp
                ${PROJECT_SOURCE_DIR}/source/business_info_page.cpp
        )

        target_include_directories(gui
                PRIVATE
                ${PROJECT_SOURCE_DIR}/include/
                ${GTKMM_INCLUDE_DIRS}
                PUBLIC
                ${CMAKE_SOURCE_DIR}/app/include
        )
else ()
        message(STATUS "There are no files in, gui/source.")
endif()
