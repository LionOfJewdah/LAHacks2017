#############################################################################
# Makefile for building: Hash Tables
# David Silverstone
# Nov. 27, 2016
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
STD			  		= -std=c++11
CLARGS		  	=
DEFINES       = $(CLARGS)
CFLAGS        = -m64 -pipe -O2 -g -Wall -W $(DEFINES)
CXXFLAGS      = -m64 -pipe -O2 $(STD) -g -Wall -W $(DEFINES)
LINK          = g++
LFLAGS        = -m64 -Wl,-O1
LIBS		  		= -L/usr/lib/x86_x64-linux-gnu -lpthread $(THREADING)
GTEST_LL	  	= -I /usr/include/gtest/ -l gtest -l gtest_main -pthread
AR            = ar cqs
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
THREADING     = -pthread


####### Output directory

BIN_DIR   	  = bin
TEST_DIR	  	= tests
# TARGET        = cube.o
#directory for reports from counting.cpp
REPT_DIR 			= reports
#directory for (long) files from calling Hashtable::reportAll()
OUTF_DIR			= cubeOutput

####### Files

SOURCES       = Cube.cpp cube_test.cpp

HEADERS		  	= Cube.hpp

TEST_SOURCES  = gtest_cube.cpp harder_gtests.cpp

OBJECTS   		= cube_test.o cube.o

EXECS		  		= $(BIN_DIR)/cube_test.out #$(BIN_DIR)/$(TARGET)

TEST_EXECS    = $(TEST_DIR)/harder_gtests.out $(TEST_DIR)/gtest_cube.out

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

ifdef TARGET
all: $(TARGET)
	gdb $(TARGET)
else
all: cube.o
endif

cube.o: Hashtable.cpp Cube.hpp
		$(CXX) -c $(CXXFLAGS) -o $@ $<

counting.o: counting.cpp Cube.hpp
		$(CXX) -c $(CXXFLAGS) -o $@ $<

main.o: main.cpp Cube.hpp
		$(CXX) -c $(CXXFLAGS) -o $@ $<

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
