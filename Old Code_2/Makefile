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
TEST_DIR	  	= tests
# TARGET        = cubeTest
#directory for (long) files from calling Hashtable::reportAll()
OUTF_DIR			= cubeOutput

####### Files

SOURCES       = cube.cpp

HEADERS		  	= cube.h

TEST_SOURCES  = cubeTest.cpp

OBJECTS   		= $(BIN_DIR)/cube.o

EXECS		  = $(TEST_DIR)/cubeTest #$(BIN_DIR)/$(TARGET)

TEST_EXECS    = $(TEST_DIR)/cubeTest

first: all
####### Implicit rules

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

ifdef TARGET
all: $(TARGET)
	$(TARGET)
else
all: cubeTest
endif

$(BIN_DIR)/cube.o: cube.cpp cube.h $(BIN_DIR)/.dirstamp
	$(CXX) -c $(CXXFLAGS) $(CLDEFINES) -o $@ $<

cubeTest: cubeTest.cpp $(BIN_DIR)/cube.o
	$(CXX) $(CXXFLAGS) -o $(TEST_DIR)/$@ $^ $(GTEST_LL)
	$(TEST_DIR)/$@

MAIN_OBJECTS = main.o cube.o


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
