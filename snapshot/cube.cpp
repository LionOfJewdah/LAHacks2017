//
//  Cube.cpp
//  LAHacks2017
//
//  Created by David Paul Silverstone on Sat, Apr 1st, 2017.
//
//

#include "cube.h"
#include <cstdlib>
#include <utility>
#ifdef DEBUG
#include <iostream>
constexpr char* cubieNames[] =  { /*0*/ "",
    /*1*/   "[1]: white-orange-green corner",
    /*2*/   "[2]: white-green edge",
    /*3*/   "[3]: white-green-red corner",
    /*4*/   "[4]: white-red edge",
    /*5*/   "[5]: white-red-blue corner",
    /*6*/   "[6]: white-blue edge",
    /*7*/   "[7]: white-blue-orange corner",
    /*8*/   "[8]: white-orange edge",
    /*9*/   "[9]: orange-green edge",
    /*10*/  "[10]: green-red edge",
    /*11*/  "[11]: red-blue edge",
    /*12*/  "[12]: blue-orange edge",
    /*13*/  "[13]: yellow-orange-green corner",
    /*14*/  "[14]: green-yellow edge",
    /*15*/  "[15]: yellow-green-red corner",
    /*16*/  "[16]: red-yellow edge",
    /*17*/  "[17]: yellow-red-blue corner",
    /*18*/  "[18]: blue-yellow edge",
    /*19*/  "[19]: yellow-blue-orange corner",
    /*20*/  "[20]: orange-yellow edge",
};

#define SAY_MOVING(a, b, c, d, e, f, g, h)\
    std::cout << "Moving " << names[mCorners[(a)]] << " --> "\
        << names[mCorners[(b)]] << " --> " << names[mCorners[(c)]] << " --> "\
        << names[mCorners[(d)]] << " --> " << names[mCorners[(a)]] << ";\nand"\
        << names[mEdges[(e)]] << " --> " << names[mCorners[(f)]] << " --> "\
        << names[mEdges[(g)]] << " --> " << names[mCorners[(h)]] << " --> "\
        << names[mEdges[(e)]] << "." << std::endl;

#define SAY_2MOVING(a, b, c, d, e, f, g, h)\
    std::cout << "Moving " << names[mCorners[(a)]] << " <--> "\
        << names[mCorners[(c)]] << " ; " << names[mCorners[(b)]] << " <--> "\
        << names[mCorners[(d)]] << ";\nand"\
        << names[mEdges[(e)]] << " <--> " << names[mCorners[(g)]] << " ; "\
        << names[mEdges[(f)]] << " <--> " << names[mCorners[(h)]] << "." << std::endl;

#endif // DEBUG

extern const Cube totallySolvedCube;
const Cube totallySolvedCube;

template <typename T, std::size_t N>
constexpr std::size_t array_size (const T (&arr)[N]) {
    return N;
}

constexpr int solved_corner_config[] = {1, 3, 5, 7, 13, 15, 17, 19};
constexpr int solved_edge_config[]   = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18,
    20};

Cube::Cube() : //mCorners({1, 3, 5, 7, 13, 15, 18, 19}),
    //mEdges({2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20}),
    _isSolved(true)
{
    for (auto i = 0; i < 8; i++)  mCorners[i] = (corner)solved_corner_config[i];
    for (auto i = 0; i < 12; i++) mEdges[i] = (edge_piece)solved_edge_config[i];
    // isSolved = true;
}

Cube::Cube(bool b) : _isSolved(true) {
    for (auto i = 0; i < 8; i++)  mCorners[i] = (corner)solved_corner_config[i];
    for (auto i = 0; i < 12; i++) mEdges[i] = (edge_piece)solved_edge_config[i];
    if (b == RANDOM_CUBE) {
        this->randomPermute();
        checkSolved();
    }
}

Cube::Cube(const int (&_corners)[8], const int (&_edges)[12])
    : //mCorners({1, 3, 5, 7, 13, 15, 18, 19}),
    //mEdges({2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20}),
    _isSolved(false)
{
    for (auto i = 0; i < 8; i++)  mCorners[i] = (corner) _corners[i];
    for (auto i = 0; i < 12; i++) mEdges[i]   = (edge_piece) _edges[i];
    checkSolved();
}

bool Cube::isSolved() const
{
	return _isSolved;
}

//typename Cube::edge_piece
// const int (&Cube::get_edges() const) [12]
const typename Cube::edge_piece (&Cube::get_edges() const) [12]
{
	return mEdges;
}

//typename Cube::corner
// const int (&Cube::get_corners() const) [8]
const typename Cube::corner (&Cube::get_corners() const) [8]
{
	return mCorners;
}

void Cube::checkSolved() {
    for(int i = 0; i < 8; i++) {
        if (solved_corner_config[i] != mCorners[i]) {
            _isSolved = false;
            return;
        }
    }

    for(int i = 0; i < 12; i++) {
        if (solved_edge_config[i] != mEdges[i]) {
            _isSolved = false;
            return;
        }
    }
    _isSolved = true;
}

bool Cube::operator==(const Cube& rhs) const {
    for (auto i = 0; i < 8; i++) {
        if (mCorners[i] != rhs.mCorners[i]) return false;
        if (mEdges[i] != rhs.mEdges[i]) return false;
    }
    for (auto i = 8; i < 12; i++) {
        if (mEdges[i] != rhs.mEdges[i]) return false;
    }
    return true;
}

Cube& Cube::turnUp()
{
    #ifdef DEBUG
    std::cout << "Performing U rotation (turnUp())." << std::endl;
    SAY_MOVING(0, 1, 2, 3, 0, 1, 2, 3);
    #endif
    quarterTurn(mCorners[0], mCorners[1], mCorners[2], mCorners[3]);
    quarterTurn(mEdges[0], mEdges[1], mEdges[2], mEdges[3]);
    checkSolved();
	return *this;
}

Cube& Cube::turnUp2()
{
    #ifdef DEBUG
    std::cout << "Performing U2 rotation (turnUp2())." << std::endl;
    SAY_2MOVING(0, 1, 2, 3, 0, 1, 2, 3);
    #endif
    halfTurn(mCorners[0], mCorners[1], mCorners[2], mCorners[3]);
    halfTurn(mEdges[0], mEdges[1], mEdges[2], mEdges[3]);
    checkSolved();
	return *this;
}

Cube& Cube::turnUpI()
{
    #ifdef DEBUG
    std::cout << "Performing U' rotation (turnUpI())." << std::endl;
    SAY_MOVING(3, 2, 1, 0, 3, 2, 1, 0);
    #endif
    quarterTurn(mCorners[3], mCorners[2], mCorners[1], mCorners[0]);
    quarterTurn(mEdges[3], mEdges[2], mEdges[1], mEdges[0]);
    checkSolved();
	return *this;
}

Cube& Cube::turnDown()
{
    #ifdef DEBUG
    std::cout << "Performing D rotation (turnDown())." << std::endl;
    SAY_MOVING(7, 6, 5, 4, 10, 9, 8, 11);
    #endif
    quarterTurn(mCorners[7], mCorners[6], mCorners[5], mCorners[4]);
    quarterTurn(mEdges[10], mEdges[9], mEdges[8], mEdges[11]);
    checkSolved();
	return *this;
}

Cube& Cube::turnDown2()
{
    #ifdef DEBUG
    std::cout << "Performing D2 rotation (turnDown2())." << std::endl;
    SAY_2MOVING(7, 6, 5, 4, 10, 9, 8, 11);
    #endif
    halfTurn(mCorners[7], mCorners[6], mCorners[5], mCorners[4]);
    halfTurn(mEdges[10], mEdges[9], mEdges[8], mEdges[11]);
    checkSolved();
	return *this;
}

Cube& Cube::turnDownI()
{
    #ifdef DEBUG
    std::cout << "Performing D' rotation (turnDownI())." << std::endl;
    SAY_MOVING(4, 5, 6, 7, 11, 8, 9, 10);
    #endif
    quarterTurn(mCorners[4], mCorners[5], mCorners[6], mCorners[7]);
    quarterTurn(mEdges[11], mEdges[8], mEdges[9], mEdges[10]);
    checkSolved();
	return *this;
}

Cube& Cube::turnFront()
{
    #ifdef DEBUG
    std::cout << "Performing F rotation (turnFront())." << std::endl;
    SAY_MOVING(4, 5, 1, 0, 8, 5, 0, 4);
    #endif
    quarterTurn(mCorners[4], mCorners[5], mCorners[1], mCorners[0]);
    quarterTurn(mEdges[8], mEdges[5], mEdges[0], mEdges[4]);
    checkSolved();
	return *this;
}

Cube& Cube::turnFront2()
{
    #ifdef DEBUG
    std::cout << "Performing F2 rotation (turnFront2())." << std::endl;
    SAY_2MOVING(4, 5, 1, 0, 8, 5, 0, 4);
    #endif
    halfTurn(mCorners[4], mCorners[5], mCorners[1], mCorners[0]);
    halfTurn(mEdges[8], mEdges[5], mEdges[0], mEdges[4]);
    checkSolved();
	return *this;
}

Cube& Cube::turnFrontI()
{
    #ifdef DEBUG
    std::cout << "Performing F' rotation (turnFrontI())." << std::endl;
    SAY_MOVING(0, 1, 5, 4, 4, 0, 5, 8);
    #endif
    quarterTurn(mCorners[0], mCorners[1], mCorners[5], mCorners[4]);
    quarterTurn(mEdges[4], mEdges[0], mEdges[5], mEdges[8]);
    checkSolved();
	return *this;
}

Cube& Cube::turnBack()
{
    #ifdef DEBUG
    std::cout << "Performing B rotation (turnBack())." << std::endl;
    SAY_MOVING(6, 7, 3, 2, 10, 7, 2, 6);
    #endif
    quarterTurn(mCorners[6], mCorners[7], mCorners[3], mCorners[2]);
    quarterTurn(mEdges[10], mEdges[7], mEdges[2], mEdges[6]);
    checkSolved();
	return *this;
}

Cube& Cube::turnBack2()
{
    #ifdef DEBUG
    std::cout << "Performing B2 rotation (turnBack2())." << std::endl;
    SAY_2MOVING(6, 7, 3, 2, 10, 7, 2, 6);
    #endif
    halfTurn(mCorners[6], mCorners[7], mCorners[3], mCorners[2]);
    halfTurn(mEdges[10], mEdges[7], mEdges[2], mEdges[6]);
    checkSolved();
	return *this;
}

Cube& Cube::turnBackI()
{
    #ifdef DEBUG
    std::cout << "Performing B' rotation (turnBackI())." << std::endl;
    SAY_MOVING(2, 3, 7, 6, 6, 2, 7, 10);
    #endif
    quarterTurn(mCorners[2], mCorners[3], mCorners[7], mCorners[6]);
    quarterTurn(mEdges[6], mEdges[2], mEdges[7], mEdges[10]);
    checkSolved();
	return *this;
}

Cube& Cube::turnLeft()
{
    #ifdef DEBUG
    std::cout << "Performing L rotation (turnLeft())." << std::endl;
    SAY_MOVING(7, 4, 0, 3, 11, 4, 3, 7);
    #endif
    quarterTurn(mCorners[7], mCorners[4], mCorners[0], mCorners[3]);
    quarterTurn(mEdges[11], mEdges[4], mEdges[3], mEdges[7]);
    checkSolved();
	return *this;
}

Cube& Cube::turnLeft2()
{
    #ifdef DEBUG
    std::cout << "Performing L2 rotation (turnLeft2())." << std::endl;
    SAY_2MOVING(7, 4, 0, 3, 11, 4, 3, 7);
    #endif
    halfTurn(mCorners[7], mCorners[4], mCorners[0], mCorners[3]);
    halfTurn(mEdges[11], mEdges[4], mEdges[3], mEdges[7]);
    checkSolved();
	return *this;
}

Cube& Cube::turnLeftI()
{
    #ifdef DEBUG
    std::cout << "Performing L' rotation (turnLeftI())." << std::endl;
    SAY_MOVING(3, 0, 4, 7, 7, 3, 4, 11);
    #endif
    quarterTurn(mCorners[3], mCorners[0], mCorners[4], mCorners[7]);
    quarterTurn(mEdges[7], mEdges[3], mEdges[4], mEdges[11]);
    checkSolved();
	return *this;
}

Cube& Cube::turnRight()
{
    #ifdef DEBUG
    std::cout << "Performing R rotation (turnRight())." << std::endl;
    SAY_MOVING(5, 6, 2, 1, 9, 6, 1, 5);
    #endif
    quarterTurn(mCorners[5], mCorners[6], mCorners[2], mCorners[1]);
    quarterTurn(mEdges[9], mEdges[6], mEdges[1], mEdges[5]);
    checkSolved();
	return *this;
}

Cube& Cube::turnRight2()
{
    #ifdef DEBUG
    std::cout << "Performing R2 rotation (turnRight2())." << std::endl;
    SAY_2MOVING(5, 6, 2, 1, 9, 6, 1, 5);
    #endif
    halfTurn(mCorners[5], mCorners[6], mCorners[2], mCorners[1]);
    halfTurn(mEdges[9], mEdges[6], mEdges[1], mEdges[5]);
    checkSolved();
	return *this;
}

Cube& Cube::turnRightI()
{
    #ifdef DEBUG
    std::cout << "Performing R' rotation (turnRightI())." << std::endl;
    SAY_MOVING(1, 2, 6, 5, 5, 1, 6, 9);
    #endif
    quarterTurn(mCorners[1], mCorners[2], mCorners[6], mCorners[5]);
    quarterTurn(mEdges[5], mEdges[1], mEdges[6], mEdges[9]);
    checkSolved();
	return *this;
}

inline int rand_up_to(const unsigned n) {
    return std::rand() % n;
}

void Cube::randomPermute() {
    using Rot = Cube& (Cube::*)(void);
    static constexpr Rot rotations[] = {
        &Cube::turnUp2, &Cube::turnUpI, &Cube::turnDown, &Cube::turnDown2,
        &Cube::turnDownI, &Cube::turnFront, &Cube::turnFront2,
        &Cube::turnFrontI, &Cube::turnBack, &Cube::turnBack2,
        &Cube::turnBackI, &Cube::turnLeft, &Cube::turnLeft2, &Cube::turnLeftI,
        &Cube::turnRight, &Cube::turnRight2, &Cube::turnRightI
    };
    static constexpr auto numRotTypes = array_size(rotations);
    // randomize with 100 moves, chosen at random
    for (int i = 0; i < 100; i++) {
        Rot r = rotations[rand_up_to(numRotTypes)];
        (this->*r)();
    }
    checkSolved();
    // if not solved, permute again, 20 times, until not solved
    while (this->isSolved()) {
        for (int i = 0; i < 20; i++) {
            Rot r = rotations[rand_up_to(numRotTypes)];
            (this->*r)();
        }
        checkSolved();
    }
}
