##############################################################
# Contents: Paths to source files used by CppUTest
# Author: Dawid Blom.
# Date: September 18, 2022
#
# NOTE: This Makefile adds value to the CppUTests framework
# by specifying the folders where the code is to be tested.
##############################################################
PROJECT_DIR :=$(realpath ..)
TEST_DIR :=$(realpath .)
TEST_TARGET := admin-system
CPPUTEST_EXE_FLAGS :=-c

########################################################################
# Uncomment to exclude the email_test group from being tested.
#
# NOTE: Ensure this line is commented when pushing code to the remote.
# 
CPPUTEST_EXE_FLAGS +=-xg "email_test"

CPPUTEST_CXXFLAGS :=-std=c++23 
CPPUTEST_CFLAGS := $(shell pkg-config -cflags cairo) $(shell curl-config --cflags)
CPPUTEST_LDFLAGS := $(shell pkg-config --libs cairo) $(shell curl-config --libs)



SRC_DIRS :=\
	$(PROJECT_DIR)/data/source\
	$(PROJECT_DIR)/utility/source\
	$(PROJECT_DIR)/storage/source\
	$(PROJECT_DIR)/features/source\


INCLUDE_DIRS :=\
	$(CPPUTEST_HOME)/include\
	$(PROJECT_DIR)/app/include\
	$(PROJECT_DIR)/data/include\
	$(PROJECT_DIR)/utility/include\
	$(PROJECT_DIR)/storage/include\
	$(PROJECT_DIR)/storage/mocks/include\
	$(PROJECT_DIR)/features/include\
	$(PROJECT_DIR)/features/mocks/include\


TEST_SRC_DIRS :=\
	$(TEST_DIR)\
	$(PROJECT_DIR)/data/tests\
	$(PROJECT_DIR)/utility/tests\
	$(PROJECT_DIR)/storage/tests\
	$(PROJECT_DIR)/features/tests\


MOCKS_SRC_DIRS := \
	$(PROJECT_DIR)/utility/mocks/source\
	$(PROJECT_DIR)/storage/mocks/source\
	$(PROJECT_DIR)/features/mocks/source\


CPPUTEST_USE_EXTENSIONS := Y
CPPUTEST_USE_GCOV := Y
CPPUTEST_USE_VPATH := Y
CPPUTEST_USE_STD_CPP_LIB := Y
CPPUTEST_USE_MEM_LEAK_DETECTION := N
-include $(CPPUTEST_HOME)/build/MakefileWorker.mk
