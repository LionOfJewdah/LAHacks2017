# Project: Rubik

CPP      = g++ -D__DEBUG__
CC       = gcc -D__DEBUG__
OBJ      = main.o Cube.o Exception.o CubieModel.o MemMgm.o
LINKOBJ  = main.o Cube.o Exception.o CubieModel.o MemMgm.o
LIBS     = -static-libgcc -m32 -g3
INCS     =
CXXINCS  = 
BIN      = rubik
CXXFLAGS = $(CXXINCS) -m32 -std=c++0x -Wall -pedantic -g3
CFLAGS   = $(INCS) -m32 -std=c++0x -Wall -pedantic -g3
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o $(BIN) $(LIBS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o $(CXXFLAGS)

Cube.o: Cube.cpp
	$(CPP) -c Cube.cpp -o Cube.o $(CXXFLAGS)

Exception.o: Exception.cpp
	$(CPP) -c Exception.cpp -o Exception.o $(CXXFLAGS)

CubieModel.o: CubieModel.cpp
	$(CPP) -c CubieModel.cpp -o CubieModel.o $(CXXFLAGS)

MemMgm.o: MemMgm.cpp
	$(CPP) -c MemMgm.cpp -o MemMgm.o $(CXXFLAGS)
