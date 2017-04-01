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
public:
    Cube();
    ~Cube();
};


#endif /* Cube_hpp */
