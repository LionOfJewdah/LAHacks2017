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

####### Output directory
BIN_DIR   	  = bin
TEST_DIR	  	= tests
# TARGET        = cube.o
#directory for reports from counting.cpp
REPT_DIR 			= reports
#directory for (long) files from calling Hashtable::reportAll()
OUTF_DIR			= cubeOutput

####### Files

SOURCES       = Cube.cpp

HEADERS		  	= cube.h

TEST_SOURCES  = cubeTest.cpp

OBJECTS   		= cube.o

EXECS		  		= $(BIN_DIR)/cubeTest.out #$(BIN_DIR)/$(TARGET)

TEST_EXECS    = $(TEST_DIR)/cubeTest.out

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

ifdef TARGET
all: $(TARGET)
	gdb $(TARGET)
else
all: cube.o cubeTest.cpp
endif

cube.o: cube.cpp cube.h
	$(CXX) -c $(CXXFLAGS) -o $@ $<

cubeTest: cube.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

MAIN_OBJECTS = main.o cube.o

main.out: $(MAIN_OBJECTS) $(HEADERS) $(BIN_DIR)/.dirstamp
		$(LINK) $(LFLAGS) -o $(BIN_DIR)/$@ $(MAIN_OBJECTS)
		gdb $(BIN_DIR)/$@

COUNT_OBJECTS = counting.o cube.o
COUNT_DIRSTAMPS = $(BIN_DIR)/.dirstamp $(REPT_DIR)/.dirstamp \
 									$(OUTF_DIR)/.dirstamp

counting.out: $(COUNT_OBJECTS) $(COUNT_DIRSTAMPS)
		$(LINK) $(LFLAGS) -o $(BIN_DIR)/$@ $(COUNT_OBJECTS)
		-@echo "Remember, make [output] "$(OUTF_DIR)"/[file]."
		-@echo "It will help to run with \`<args> >> "$(REPT_DIR)"/[result]\`, too."

counting: counting.out
		cp $(BIN_DIR)/$< $@

goCount.sh: _counting.txt
	cp $< $@
	chmod +x $@

VERBOSE_LOG 	=  $(REPT_DIR)/countingRuntimes.txt
MINIMAL_LOG 	=  $(REPT_DIR)/bareTimes.txt
COUNT_LOGS 		=  $(VERBOSE_LOG) $(MINIMAL_LOG)

# generates my reports every time I change counting.cpp or Hashtable.cpp
count: goCount.sh counting
		$(MOVE) $(VERBOSE_LOG) $(VERBOSE_LOG).bak
		$(MOVE) $(MINIMAL_LOG) $(MINIMAL_LOG).bak
		./$<

gtest_cube.out: gtest_cube.cpp Cube.hpp cube.o $(TEST_DIR)/.dirstamp
		$(CXX) -g -Wall $(STD) $(CLARGS) -o $(TEST_DIR)/$@ $< $(GTEST_LL) \
					cube.o
		gdb $(TEST_DIR)/$@

harder_gtests.out: harder_gtests.cpp Cube.hpp cube.o $(TEST_DIR)/.dirstamp
		$(CXX) -g -Wall $(STD) $(CLARGS) -o $(TEST_DIR)/$@ $< $(GTEST_LL) \
					cube.o
		gdb $(TEST_DIR)/$@

#======================================
# cleaning tools and dirstamps

clean:
	rm -f $(EXECS) $(OBJECTS)


$(BIN_DIR)/.dirstamp:
	-@mkdir -p $(BIN_DIR)
	-@touch $@

$(REPT_DIR)/.dirstamp:
	-@mkdir -p $(REPT_DIR)
	-@touch $@

$(OUTF_DIR)/.dirstamp:
	-@mkdir -p $(OUTF_DIR)
	-@touch $@

$(TEST_DIR)/.dirstamp:
	-@mkdir -p $(TEST_DIR)
	-@touch $@
