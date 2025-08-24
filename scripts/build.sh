############################################################################
# Contents: Project Build Functions
# Author: Dawid Blom
# Date: September 15, 2023
#
# NOTE:
############################################################################
#!/bin/bash


PROJECT_VERSION_NUM=0.0.1
PROJECT_VERSION_PREFIX=-v
BIN_SUFFIX=
CMAKE=cmake
BUILD_TYPE="Debug"
BUILD_DIR=build
PROJECT_NAME=$(basename `pwd`)
BIN_DIR=$BUILD_DIR/$PROJECT_NAME
SUPPORTED_BUILD_TYPES=("host" "deploy")

function run_build()
{
        local build_type=$1
        remove_directory $BUILD_DIR
        if [[ $build_type == ${SUPPORTED_BUILD_TYPES[0]} ]];
        then
                $ECHO "${INFO_COLOR}Building for the host platform${END_COLOR}"
                host_build
        elif [[ $build_type == ${SUPPORTED_BUILD_TYPES[1]} ]];
        then
                $ECHO "${INFO_COLOR}Building for the target platform${END_COLOR}"
                deploy_build
        else
                $ECHO "${ERROR_COLOR}ERROR: Build type not supported${END_COLOR}"
        fi
}

function host_build()
{
        mkdir -p $BIN_DIR
        $CMAKE -S . -B $BIN_DIR --warn-uninitialized -DCMAKE_BUILD_TYPE=$BUILD_TYPE  \
                -DCMAKE_PROJECT_NAME=$PROJECT_NAME  \
                -DCMAKE_EXECUTABLE_SUFFIX=$BIN_SUFFIX  \
                -DBUILD_PROJECT=ON
        $CMAKE --build $BIN_DIR
}

function deploy_build()
{
        $CMAKE -S . -B $BIN_DIR --warn-uninitialized -DCMAKE_BUILD_TYPE=$BUILD_TYPE  \
                -DCMAKE_PROJECT_NAME=$PROJECT_NAME  \
                -DCMAKE_PROJECT_VERSION=$PROJECT_VERSION_PREFIX$PROJECT_VERSION_NUM  \
                -DCMAKE_EXECUTABLE_SUFFIX=$BIN_SUFFIX  \
                -DBUILD_PROJECT_FOR_DEPLOY=ON  \
                -DBUILD_PROJECT=ON
        $CMAKE --build $BIN_DIR

        DEPLOY_DIR=$BUILD_DIR/$PROJECT_NAME$PROJECT_VERSION_PREFIX$PROJECT_VERSION_NUM
        mkdir -p $DEPLOY_DIR
        cp -r $BIN_DIR/$PROJECT_NAME$PROJECT_VERSION_PREFIX$PROJECT_VERSION_NUM$BIN_SUFFIX $DEPLOY_DIR
        tar cvf $DEPLOY_DIR.tar.gz $DEPLOY_DIR
}
