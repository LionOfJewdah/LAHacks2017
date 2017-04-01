//
//  Cube.hpp
//  LAHacks2017
//
//  Created by David Paul Silverstone on Sat, Apr 1st, 2017.
//
//

#ifndef Cube_hpp
#define Cube_hpp

#include <algorithm>

// A class representing the states of a rubix cube
class Cube {
private:
    enum corner {
        WOG = 1,  WGR = 3,  WRB = 5,  WBO = 7,
        YOG = 13, YGR = 15, YRB = 17, YBO = 19
    };

    enum edge_piece {
        WG = 2,  WR = 4,  WB = 6,  WO = 8,
        OG = 9,  GR = 10, RB = 11, BO = 12,
        GY = 14, RY = 16, BY = 18, OY = 20
    };
public:
    enum { SOLVED_CUBE = false, RANDOM_CUBE = true };
    Cube();
    ~Cube();
};


#endif /* Cube_hpp */
