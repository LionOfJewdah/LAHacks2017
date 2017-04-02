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
        /*1*/  "[1]: white-orange-green corner",

        /*3*/  "[3]: white-green-red corner",

        /*5*/  "[5]: white-red-blue corner",

        /*7*/  "[7]: white-blue-orange corner",

        /*13*/ "[13]: yellow-orange-green corner",

        /*15*/ "[15]: yellow-green-red corner",

        /*17*/ "[17]: yellow-red-blue corner",

        /*19*/ "[19]: yellow-blue-orange corner",

    };

    enum edge_piece {
        WG = 2,  WR = 4,  WB = 6,  WO = 8,
        OG = 9,  GR = 10, RB = 11, BO = 12,
        GY = 14, RY = 16, BY = 18, OY = 20
    };
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
    std::cout << "Performing U rotation (turnUp()). Moving " << std::endl;
    #endif
    quarterTurn(mCorners[0], mCorners[1], mCorners[2], mCorners[3]);
    quarterTurn(mEdges[0], mEdges[1], mEdges[2], mEdges[3]);
    checkSolved();
	return *this;
}

Cube& Cube::turnUp2()
{
    halfTurn(mCorners[0], mCorners[1], mCorners[2], mCorners[3]);
    halfTurn(mEdges[0], mEdges[1], mEdges[2], mEdges[3]);
    checkSolved();
	return *this;
}

Cube& Cube::turnUpI()
{
    quarterTurn(mCorners[3], mCorners[2], mCorners[1], mCorners[0]);
    quarterTurn(mEdges[3], mEdges[2], mEdges[1], mEdges[0]);
    checkSolved();
	return *this;
}

Cube& Cube::turnDown()
{
    quarterTurn(mCorners[7], mCorners[6], mCorners[5], mCorners[4]);
    quarterTurn(mEdges[10], mEdges[9], mEdges[8], mEdges[11]);
    checkSolved();
	return *this;
}

Cube& Cube::turnDown2()
{
    halfTurn(mCorners[7], mCorners[6], mCorners[5], mCorners[4]);
    halfTurn(mEdges[10], mEdges[9], mEdges[8], mEdges[11]);
    checkSolved();
	return *this;
}

Cube& Cube::turnDownI()
{
    quarterTurn(mCorners[4], mCorners[5], mCorners[6], mCorners[7]);
    quarterTurn(mEdges[11], mEdges[8], mEdges[9], mEdges[10]);
    checkSolved();
	return *this;
}

Cube& Cube::turnFront()
{
    quarterTurn(mCorners[4], mCorners[5], mCorners[1], mCorners[0]);
    quarterTurn(mEdges[8], mEdges[5], mEdges[0], mEdges[4]);
    checkSolved();
	return *this;
}

Cube& Cube::turnFront2()
{
    halfTurn(mCorners[4], mCorners[5], mCorners[1], mCorners[0]);
    halfTurn(mEdges[8], mEdges[5], mEdges[0], mEdges[4]);
    checkSolved();
	return *this;
}

Cube& Cube::turnFrontI()
{
    quarterTurn(mCorners[0], mCorners[1], mCorners[5], mCorners[4]);
    quarterTurn(mEdges[4], mEdges[0], mEdges[5], mEdges[8]);
    checkSolved();
	return *this;
}

Cube& Cube::turnBack()
{
    quarterTurn(mCorners[6], mCorners[7], mCorners[3], mCorners[2]);
    quarterTurn(mEdges[10], mEdges[7], mEdges[2], mEdges[6]);
    checkSolved();
	return *this;
}

Cube& Cube::turnBack2()
{
    halfTurn(mCorners[6], mCorners[7], mCorners[3], mCorners[2]);
    halfTurn(mEdges[10], mEdges[7], mEdges[2], mEdges[6]);
    checkSolved();
	return *this;
}

Cube& Cube::turnBackI()
{
    quarterTurn(mCorners[2], mCorners[3], mCorners[7], mCorners[6]);
    quarterTurn(mEdges[6], mEdges[2], mEdges[7], mEdges[10]);
    checkSolved();
	return *this;
}

Cube& Cube::turnLeft()
{
    quarterTurn(mCorners[7], mCorners[4], mCorners[0], mCorners[3]);
    quarterTurn(mEdges[11], mEdges[4], mEdges[3], mEdges[7]);
    checkSolved();
	return *this;
}

Cube& Cube::turnLeft2()
{
    halfTurn(mCorners[7], mCorners[4], mCorners[0], mCorners[3]);
    quarterTurn(mEdges[11], mEdges[4], mEdges[3], mEdges[7]);
    checkSolved();
	return *this;
}

Cube& Cube::turnLeftI()
{
    quarterTurn(mCorners[3], mCorners[0], mCorners[4], mCorners[7]);
    quarterTurn(mEdges[7], mEdges[3], mEdges[4], mEdges[11]);
    checkSolved();
	return *this;
}

Cube& Cube::turnRight()
{
    quarterTurn(mCorners[5], mCorners[6], mCorners[2], mCorners[1]);
    quarterTurn(mEdges[9], mEdges[6], mEdges[1], mEdges[5]);
    checkSolved();
	return *this;
}

Cube& Cube::turnRight2()
{
    halfTurn(mCorners[5], mCorners[6], mCorners[2], mCorners[1]);
    halfTurn(mEdges[9], mEdges[6], mEdges[1], mEdges[5]);
    checkSolved();
	return *this;
}

Cube& Cube::turnRightI()
{
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
    while (this->is_solved()) {
        for (int i = 0; i < 20; i++) {
            Rot r = rotations[rand_up_to(numRotTypes)];
            (this->*r)();
        }
        checkSolved();
    }
}
