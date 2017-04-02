#############################################################################
# Makefile for building: Cube Solver
#############################################################################

####### Compiler, tools and options
CC            = gcc
CXX           = g++
STD			  = -std=c++11
CXXFLAGS      = $(STD) -g -Wall -W
GTEST_LL	  = -I /usr/include/gtest/ -l gtest -l gtest_main -pthread
DEL_FILE      = rm -f
DEL_DIR       = rmdir
MOVE          = mv -f
MKDIR         = mkdir -p
CLDEFINES	  =

####### Output directory
BIN_DIR   	  = bin
TEST_DIR	  	= ../tests
# TARGET        = cubeTest
#directory for (long) files from calling Hashtable::reportAll()
OUTF_DIR			= ../cubeOutput

####### Files

SOURCES       = cubieCube.cpp coordinateCube.cpp

HEADERS		  	= cubieCube.h coordinateCube.h

first: all
####### Implicit rules

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules


#======================================
# cleaning tools and dirstamps

clean:
	rm -f $(EXECS) $(OBJECTS)

$(BIN_DIR)/.dirstamp:
	-@mkdir -p $(BIN_DIR)
	-@touch $@

$(OUTF_DIR)/.dirstamp:
	-@mkdir -p $(OUTF_DIR)
	-@touch $@

$(TEST_DIR)/.dirstamp:
	-@mkdir -p $(TEST_DIR)
	-@touch $@
