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
project(target-controllers VERSION 0.0.1 LANGUAGES CXX C)


file(GLOB RESULT ${PROJECT_SOURCE_DIR}/source)
list(LENGTH RESULT FILES_IN_DIR)
if(${FILES_IN_DIR} GREATER 0)
        add_library(controllers
                OBJECT
                ${PROJECT_SOURCE_DIR}/source/admin_controller.cpp
                ${PROJECT_SOURCE_DIR}/source/client_invoice.cpp
                ${PROJECT_SOURCE_DIR}/source/client_register.cpp
                ${PROJECT_SOURCE_DIR}/source/client_statement.cpp
        )

        target_include_directories(controllers
                PRIVATE
                ${PROJECT_SOURCE_DIR}/include/
                ${CURL_INCLUDE_DIR}
                ${GTKMM_INCLUDE_DIRS}
                ${POPPLER_INCLUDE_DIRS}
                ${CAIROMM_INCLUDE_DIRS}
		${LIBSECRET_INCLUDE_DIRS}
                PUBLIC
                ${CMAKE_SOURCE_DIR}/app/include
                ${CMAKE_SOURCE_DIR}/data/include
                ${CMAKE_SOURCE_DIR}/storage/include
                ${CMAKE_SOURCE_DIR}/utility/include
                ${CMAKE_SOURCE_DIR}/features/include
        )
else ()
        message(STATUS "There are no files in, controllers/source.")
endif()
