//
//  Cube.cpp
//  LAHacks2017
//
//  Created by David Paul Silverstone on Sat, Apr 1st, 2017.
//
//

#include "cube.h"

extern constexpr Cube totallySolvedCube;
constexpr Cube totallySolvedCube;

constexpr int[] solved_corner_config = {1, 3, 5, 7, 13, 15, 17, 19};
constexpr int[] solved_edge_config = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};

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