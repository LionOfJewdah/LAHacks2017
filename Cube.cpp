//
//  Cube.cpp
//  LAHacks2017
//
//  Created by David Paul Silverstone on Sat, Apr 1st, 2017.
//
//

#include "Cube.hpp"

constexpr int[] solved_corner_config = {1, 3, 5, 7, 13, 15, 18, 19};
constexpr int[] solved_edge_config = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};

Cube::Cube() : mCorners({1, 3, 5, 7, 13, 15, 18, 19}),
    mEdges({2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20})
{
    for (auto i = 0; i < 8; i++)  mCorners[i] = solved_corner_config[i];
    for (auto i = 0; i < 12; i++) mEdges[i]   = solved_edge_config[i];
    isSolved = true;
}

bool Cube::is_solved() const
{
	return isSolved;
}

typename Cube::edge_piece* const Cube::edges()
{
	return edges;
}

typename Cube::corners* const Cube::corners()
{
	return corners;
}
