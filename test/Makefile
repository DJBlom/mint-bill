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
CPPUTEST_EXE_FLAGS := -c
CPPUTEST_CXXFLAGS :=-std=c++23


SRC_DIRS :=\
	$(PROJECT_DIR)/api/source\
	$(PROJECT_DIR)/data/source\
	$(PROJECT_DIR)/features/source\


INCLUDE_DIRS :=\
	$(CPPUTEST_HOME)/include\
	$(PROJECT_DIR)/app/include\
	$(PROJECT_DIR)/data/include\
	$(PROJECT_DIR)/features/include\
	$(PROJECT_DIR)/features/mocks/include\


TEST_SRC_DIRS :=\
	$(TEST_DIR)\
	$(PROJECT_DIR)/data/tests\
	$(PROJECT_DIR)/features/tests\


MOCKS_SRC_DIRS := \
	$(PROJECT_DIR)/api/mocks/source\
	$(PROJECT_DIR)/features/mocks/source\


CPPUTEST_USE_EXTENSIONS := Y
CPPUTEST_USE_GCOV := Y
CPPUTEST_USE_VPATH := Y
CPPUTEST_USE_MEM_LEAK_DETECTION := Y
-include $(CPPUTEST_HOME)/build/MakefileWorker.mk
