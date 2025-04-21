############################################################################
# Contents: Invoice Page CMakeLists.txt file
# Author: Dawid Blom
# Date: January 24, 2025
#
# NOTE:
############################################################################
cmake_minimum_required(VERSION 3.25)
project(target-invoice_page VERSION 0.0.1 LANGUAGES CXX C)


file(GLOB RESULT ${PROJECT_SOURCE_DIR}/source)
list(LENGTH RESULT FILES_IN_DIR)
if(${FILES_IN_DIR} GREATER 0)

        add_library(invoice_page
                OBJECT
                ${PROJECT_SOURCE_DIR}/source/invoice_page.cpp
        )

        target_include_directories(invoice_page
                PRIVATE
                ${GTKMM_INCLUDE_DIRS}
                ${CAIROMM_INCLUDE_DIRS}
                ${PROJECT_SOURCE_DIR}/include/
                PUBLIC
                ${CMAKE_SOURCE_DIR}/app/include
                ${CMAKE_SOURCE_DIR}/gui/include
                ${CMAKE_SOURCE_DIR}/data/include
                ${CMAKE_SOURCE_DIR}/storage/include
                ${CMAKE_SOURCE_DIR}/features/include
                ${CMAKE_SOURCE_DIR}/utility/include
        )
else ()
        message(STATUS "There are no files in, gui/invoice_page/source.")
endif()
