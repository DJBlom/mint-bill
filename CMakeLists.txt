############################################################################
# Contents: Top Most CMakeLists.txt file
# Author: Dawid Blom
# Date: September 15, 2023
#
# NOTE:
# This file is the main CMakeLists.txt file containing the compiler
# options and definitions, the linker options, path to Main.cpp, and
# the including of all the sub directories where other CMakeLists.txt
# files are found.
############################################################################
cmake_minimum_required(VERSION 3.25)

option(BUILD_PROJECT_FOR_DEPLOY "Build the project for deployment" OFF)
option(BUILD_PROJECT "Build the project" OFF)
option(UNIT_TESTS "Perform unit tests" OFF)
option(STATIC_CODE_ANALYSIS "Perform static code analysis" OFF)
option(CODE_COVERAGE "Perform code coverage" OFF)

if(BUILD_PROJECT_FOR_DEPLOY)
    project(${CMAKE_PROJECT_NAME}${CMAKE_PROJECT_VERSION}${CMAKE_EXECUTABLE_SUFFIX} LANGUAGES CXX C ASM)
    message("Building the project with a version tag")
else()
    project(${CMAKE_PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX} LANGUAGES CXX C ASM)
    message("Building the project without any version tag")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)


if(BUILD_PROJECT)
        find_package(CURL REQUIRED)
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)
        pkg_check_modules(CAIROMM REQUIRED cairomm-1.16)
        pkg_check_modules(POPPLER REQUIRED poppler-cpp)

        add_compile_options(
                -Wall
                -Werror
                -Wextra
                -Wconversion
                $<$<CONFIG:DEBUG>:-g3>
                $<$<CONFIG:DEBUG>:-Og>
        )

        add_compile_definitions(
                $<$<CONFIG:DEBUG>:DEBUG>
                $<$<CONFIG:DEBUG>:TRACE_ENABLED>
        )

        add_executable(${PROJECT_NAME}
                ${CMAKE_SOURCE_DIR}/app/source/main.cpp
                ${CMAKE_SOURCE_DIR}/app/source/admin_system.cpp
        )

        target_include_directories(${PROJECT_NAME} PRIVATE
                ${CURL_INCLUDE_DIR}
                ${GTKMM_INCLUDE_DIRS}
                ${CAIROMM_INCLUDE_DIRS}
                ${POPPLER_INCLUDE_DIRS}
                ${CMAKE_SOURCE_DIR}/app/include
                ${CMAKE_SOURCE_DIR}/gui/include
                ${CMAKE_SOURCE_DIR}/data/include
                ${CMAKE_SOURCE_DIR}/storage/include
                ${CMAKE_SOURCE_DIR}/features/include
                ${CMAKE_SOURCE_DIR}/utility/include
                ${CMAKE_SOURCE_DIR}/gui/invoice_page/include
        )

        target_link_options(${PROJECT_NAME} PRIVATE
                LINKER:-lpthread
                LINKER:-lrt
        )

        target_link_libraries(${PROJECT_NAME} 
                PRIVATE 
                ${CURL_LIBRARIES}
                ${GTKMM_LIBRARIES} 
                ${CAIROMM_LIBRARIES}
                ${POPPLER_LIBRARIES}
        )

        target_link_directories(${PROJECT_NAME} 
                PRIVATE 
                ${GTKMM_LIBRARY_DIRS}
                ${CAIROMM_LIBRARY_DIRS}
        )

        if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/gui)
                add_subdirectory(${CMAKE_SOURCE_DIR}/gui)
                target_link_libraries(${PROJECT_NAME} PRIVATE gui)
        else()
                message(STATUS "Directory gui, does not exist")
        endif()

        if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/data)
                add_subdirectory(${CMAKE_SOURCE_DIR}/data)
                target_link_libraries(${PROJECT_NAME} PRIVATE data)
        else()
                message(STATUS "Directory data, does not exist")
        endif()

        if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/storage)
                add_subdirectory(${CMAKE_SOURCE_DIR}/storage)
                target_link_libraries(${PROJECT_NAME} PRIVATE storage)
        else()
                message(STATUS "Directory storage, does not exist")
        endif()

        if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/features)
                add_subdirectory(${CMAKE_SOURCE_DIR}/features)
                target_link_libraries(${PROJECT_NAME} PRIVATE features ${POPPLER_LIBRARIES})
        else()
                message(STATUS "Directory features, does not exist")
        endif()

        if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/utility)
                add_subdirectory(${CMAKE_SOURCE_DIR}/utility)
                target_link_libraries(${PROJECT_NAME} PRIVATE utility)
        else()
                message(STATUS "Directory utility, does not exist")
        endif()

        if (EXISTS ${CMAKE_OBJDUMP})
                add_custom_command(
                        TARGET ${PROJECT_NAME}
                        POST_BUILD
                        COMMAND ${CMAKE_OBJDUMP} -hS $<TARGET_FILE:${PROJECT_NAME}>
                        >${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_NAME:${PROJECT_NAME}>.S
                )
        else ()
                message(STATUS "'objdump' not found: cannot generate .S file")
        endif()
endif()
