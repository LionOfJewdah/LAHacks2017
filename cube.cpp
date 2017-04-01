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
