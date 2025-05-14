############################################################################
# Contents: Project Test Functions
# Author: Dawid Blom
# Date: September 15, 2023
#
# NOTE: 
############################################################################
#!/bin/bash

readonly PROJECT_VERSION_NUM=0.0.1
readonly PROJECT_VERSION_PREFIX=-v
readonly BIN_SUFFIX=
readonly CMAKE=cmake
readonly BUILD_TYPE="Debug"
readonly BUILD_DIR=build
readonly PROJECT_NAME=$(basename `pwd`)
readonly BIN_DIR=$BUILD_DIR/${PROJECT_NAME}
readonly SUPPORTED_BUILD_TYPES=("host" "deploy")

readonly TEST_DIR=$(pwd)/test
readonly TEST_TYPES=("sca" "coverage" "unit" "show")

function run_test()
{
    local test_type=$1
    if [[ $test_type == ${TEST_TYPES[0]} ]];
    then
        $ECHO "${INFO_COLOR}INFO: Running 'Cppcheck' to perform static code analysis${END_COLOR}"
        static_code_analysis
    elif [[ $test_type == ${TEST_TYPES[1]} ]];
    then
        $ECHO "${INFO_COLOR}INFO: Running 'gcovr' to perform code coverage${END_COLOR}"
        code_coverage
    elif [[ $test_type == ${TEST_TYPES[2]} ]];
    then
        $ECHO "${INFO_COLOR}INFO: Running 'CppUTest' to perform unit-testing${END_COLOR}"
        unit_test
    elif [[ $test_type == ${TEST_TYPES[3]} ]];
    then
        $ECHO "${INFO_COLOR}INFO: Launching 'firefox' to display code coverage${END_COLOR}"
        show_code_coverage
    else
        $ECHO "${ERROR_COLOR}ERROR: Test type not supported${END_COLOR}"
    fi
}

function static_code_analysis()
{
    local prj_dir=$(pwd)
    cppcheck --enable=style \
             --enable=warning \
             --enable=performance \
             --enable=portability \
             --enable=information \
             --enable=missingInclude \
             --enable=unusedFunction \
             --library=posix \
             --std=c11 \
             --std=c++23 \
             --error-exitcode=1 \
             --platform=unix32 unix64 win32A win32W win64 \
             --suppress=missingIncludeSystem \
             --suppress=checkersReport \
             --check-level=exhaustive \
             -I $prj_dir/app/include \
             -I $prj_dir/gui/include \
             -I $prj_dir/data/include \
             -I $prj_dir/storage/include \
             -I $prj_dir/features/include \
             -I $prj_dir/gui/invoice_page/include \
             $prj_dir/app/source \
             $prj_dir/gui/source \
             $prj_dir/data/source \
             $prj_dir/storage/source \
             $prj_dir/features/source \
             $prj_dir/gui/invoice_page/source 
}

function code_coverage()
{
	local prj_dir=$(pwd)
        local coverage_dir=$BUILD_DIR/coverage
        mkdir -p $coverage_dir
        make -C $TEST_DIR -s gcov
	gcovr --exclude="^[^\/]+\/mocks\/?(?:[^\/]+\/?)*$" --exclude-throw-branches -r $prj_dir \
	--html-nested $coverage_dir/coverage.html  --txt $coverage_dir/coverage.txt

	coverage=$(grep -F "TOTAL" $coverage_dir/coverage.txt)
	# Extract the line coverage percentage
	total_coverage=($(echo "$coverage" | awk -F ' ' '{print $4}' | awk -F '%' '{print $1}'))
	threshold=80
	if [[ $total_coverage -lt $threshold ]];
	then
                $ECHO "${ERROR_COLOR}FAILED: Total coverage should be ${threshold}.0% or higher.${END_COLOR}"
                $ECHO "${INFO_COLOR}INFO: Currently, it is ${total_coverage}.0%${END_COLOR}"
        if [ -d $coverage_dir ];
        then
                rm -rf $coverage_dir
                make -C $TEST_DIR -s clean
        fi
                exit 1
	else
                $ECHO "${SUCCESS_COLOR}PASS: Total coverage is: ${total_coverage}.0%${END_COLOR}"
        if [ -d $coverage_dir ];
        then
                rm -rf $coverage_dir
                make -C $TEST_DIR -s clean
        fi
                exit 0
	fi
}

function unit_test()
{
        mkdir -p $BIN_DIR
        $CMAKE -S . -B $BIN_DIR --warn-uninitialized -DCMAKE_BUILD_TYPE=$BUILD_TYPE  \
                -DCMAKE_PROJECT_NAME=$PROJECT_NAME  \
                -DCMAKE_EXECUTABLE_SUFFIX=$BIN_SUFFIX  \
                -DUNIT_TESTS=ON
        $CMAKE --build $BIN_DIR
        ctest --test-dir $BIN_DIR

	local prj_dir=$(pwd)
        make -C $TEST_DIR -s
        make -C $TEST_DIR -s clean
}

function show_code_coverage()
{
	local prj_dir=$(pwd)
        local coverage_dir=$BUILD_DIR/coverage
        mkdir -p $coverage_dir
        make -C $TEST_DIR -s gcov
        gcovr --exclude="^[^\/]+\/mocks\/?(?:[^\/]+\/?)*$" --exclude-throw-branches -r $prj_dir \
                --html-nested $coverage_dir/coverage.html  --txt $coverage_dir/coverage.txt

        firefox $coverage_dir/coverage.html
        make -C $TEST_DIR -s clean
}
