//
//  Cube.cpp
//  LAHacks2017
//
//  Created by David Paul Silverstone on Sat, Apr 1st, 2017.
//
//

#include "cube.h"
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

extern constexpr Cube totallySolvedCube;
constexpr Cube totallySolvedCube;

constexpr int[] solved_corner_config = {1, 3, 5, 7, 13, 15, 17, 19};
constexpr int[] solved_edge_config = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18,
    20};

Cube::Cube() : //mCorners({1, 3, 5, 7, 13, 15, 18, 19}),
    //mEdges({2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20}),
    isSolved(true)
{
    for (auto i = 0; i < 8; i++)  mCorners[i] = solved_corner_config[i];
    for (auto i = 0; i < 12; i++) mEdges[i]   = solved_edge_config[i];
    // isSolved = true;
}

Cube::Cube(int* corners, int* edges) : //mCorners({1, 3, 5, 7, 13, 15, 18, 19}),
    //mEdges({2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20}),
    isSolved(true)
{
    for (auto i = 0; i < 8; i++)  mCorners[i] = corners[i];
    for (auto i = 0; i < 12; i++) mEdges[i]   = edges[i];
    checkSolved();
    // isSolved = true;
}

bool Cube::is_solved() const
{
	return isSolved;
}

const typename Cube::edge_piece (&Cube::get_edges())[12] const
{
	return mEdges;
}

const typename Cube::corners (&Cube::get_corners())[8] const
{
	return mCorners;
}

void Cube::checkSolved() {
    for(int i = 0; i < 8; i++) {
        if (solved_corner_config[i] != corners[i]) {
            isSolved = false;
            return;
        }
    }

    for(int i = 0; i < 12; i++) {
        if (solved_edge_config[i] != edges[i]) {
            isSolved = false;
            return;
        }
    }
    isSolved = true;
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
}

Cube& Cube::turnUp2()
{
    halfTurn(mCorners[0], mCorners[1], mCorners[2], mCorners[3]);
    halfTurn(mEdges[0], mEdges[1], mEdges[2], mEdges[3]);
    checkSolved();
}

Cube& Cube::turnUpI()
{
    quarterTurn(mCorners[3], mCorners[2], mCorners[1], mCorners[0]);
    quarterTurn(mEdges[3], mEdges[2], mEdges[1], mEdges[0]);
    checkSolved();
}

Cube& Cube::turnDown()
{
    quarterTurn(mCorners[7], mCorners[6], mCorners[5], mCorners[4]);
    quarterTurn(mEdges[10], mEdges[9], mEdges[8], mEdges[11]);
    checkSolved();
}

Cube& Cube::turnDown2()
{
    halfTurn(mCorners[7], mCorners[6], mCorners[5], mCorners[4]);
    halfTurn(mEdges[10], mEdges[9], mEdges[8], mEdges[11]);
    checkSolved();
}

Cube& Cube::turnDownI()
{
    quarterTurn(mCorners[4], mCorners[5], mCorners[6], mCorners[7]);
    quarterTurn(mEdges[11], mEdges[8], mEdges[9], mEdges[10]);
    checkSolved();
}

Cube& Cube::turnFront()
{
    quarterTurn(mCorners[4], mCorners[5], mCorners[1], mCorners[0]);
    quarterTurn(mEdges[8], mEdges[5], mEdges[0], mEdges[4]);
    checkSolved();
}

Cube& Cube::turnFront2()
{
    halfTurn(mCorners[4], mCorners[5], mCorners[1], mCorners[0]);
    halfTurn(mEdges[8], mEdges[5], mEdges[0], mEdges[4]);
    checkSolved();
}

Cube& Cube::turnFrontI()
{
    quarterTurn(mCorners[0], mCorners[1], mCorners[5], mCorners[4]);
    quarterTurn(mEdges[4], mEdges[0], mEdges[5], mEdges[8]);
    checkSolved();
}

Cube& Cube::turnBack()
{
    quarterTurn(mCorners[6], mCorners[7], mCorners[3], mCorners[2]);
    quarterTurn(mEdges[10], mEdges[7], mEdges[2], mEdges[6]);
    checkSolved();
}

Cube& Cube::turnBack2()
{
    halfTurn(mCorners[6], mCorners[7], mCorners[3], mCorners[2]);
    halfTurn(mEdges[10], mEdges[7], mEdges[2], mEdges[6]);
    checkSolved();
}

Cube& Cube::turnBackI()
{
    quarterTurn(mCorners[2], mCorners[3], mCorners[7], mCorners[6]);
    quarterTurn(mEdges[6], mEdges[2], mEdges[7], mEdges[10]);
    checkSolved();
}

Cube& Cube::turnLeft()
{
    quarterTurn(mCorners[7], mCorners[4], mCorners[0], mCorners[3]);
    quarterTurn(mEdges[11], mEdges[4], mEdges[3], mEdges[7]);
    checkSolved();
}

Cube& Cube::turnLeft2()
{
    halfTurn(mCorners[7], mCorners[4], mCorners[0], mCorners[3]);
    quarterTurn(mEdges[11], mEdges[4], mEdges[3], mEdges[7]);
    checkSolved();
}

Cube& Cube::turnLeftI()
{
    quarterTurn(mCorners[3], mCorners[0], mCorners[4], mCorners[7]);
    quarterTurn(mEdges[7], mEdges[3], mEdges[4], mEdges[11]);
    checkSolved();
}

Cube& Cube::turnRight()
{
    quarterTurn(mCorners[5], mCorners[6], mCorners[2], mCorners[1]);
    quarterTurn(mEdges[9], mEdges[6], mEdges[1], mEdges[5]);
    checkSolved();
}

Cube& Cube::turnRight2()
{
    halfTurn(mCorners[5], mCorners[6], mCorners[2], mCorners[1]);
    halfTurn(mEdges[9], mEdges[6], mEdges[1], mEdges[5]);
    checkSolved();
}

Cube& Cube::turnRightI()
{
    quarterTurn(mCorners[1], mCorners[2], mCorners[6], mCorners[5]);
    quarterTurn(mEdges[5], mEdges[1], mEdges[6], mEdges[9]);
    checkSolved();
}

inline int rand_up_to(const unsigned n) {
    return std::rand() % n;
}

void Cube::randomPermute() {
    using Rot = Cube& (Cube::*)(void);
    static constexpr Rot rotations[] = {
        &turnUp2, &turnUpI, &turnDown, &turnDown2, &turnDownI,
        &turnFront, &turnFront2, &turnFrontI, &turnBack,
        &turnBack2, &turnBackI, &turnLeft, &turnLeft2, &turnLeftI,
        &turnRight, &turnRight2, &turnRightI
    };
    static constexpr auto numRotTypes = array_size(rotations);
    for (int i = 0; i < 100; i__) {
        Rot r = rotations[rand_up_to(numRotTypes)];
        this->r();
    }
    checkSolved();
    while (this->is_solved()) {
        for (int i = 0; i < 20; i__) {
            Rot r = rotations[rand_up_to(numRotTypes)];
            this->r();
        }
        checkSolved();
    }
}
