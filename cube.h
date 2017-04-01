//  LAHacks2017
//
//  Created by Jake Leventhal on Sat, Apr 1st, 2017.

#ifndef CUBE_H
#define CUBE_H

// A class representing the states of a rubix cube
class Cube {
private:
    corner mCorners[8];
    edge_piece mEdges[12];
    bool isSolved;
    void checkSolved();
public:
    enum { SOLVED_CUBE = false, RANDOM_CUBE = true };

    enum corner {
        WOG = 1,  WGR = 3,  WRB = 5,  WBO = 7,
        YOG = 13, YGR = 15, YRB = 17, YBO = 19
    };

    enum edge_piece {
        WG = 2,  WR = 4,  WB = 6,  WO = 8,
        OG = 9,  GR = 10, RB = 11, BO = 12,
        GY = 14, RY = 16, BY = 18, OY = 20
    };

    Cube();
    Cube(const corners (&_corners)[8], const edge_piece (&_edges)[12]);
    Cube(const Cube&) = default;
    Cube(Cube&&) = default;
    ~Cube() = default;
    const corners  (& get_corners())[8]  const;
    const edge_piece (& get_edges())[12] const;

    bool is_solved() const;
    bool operator==(const Cube& rhs) const;

    Cube& turnUp();
    Cube& turnUp2();
    Cube& turnUpI();
    Cube& turnDown();
    Cube& turnDown2();
    Cube& turnDownI();
    Cube& turnFront();
    Cube& turnFront2();
    Cube& turnFrontI();
    Cube& turnBack();
    Cube& turnBack2();
    Cube& turnBackI();
    Cube& turnLeft();
    Cube& turnLeft2();
    Cube& turnLeftI();
    Cube& turnRight();
    Cube& turnRight2();
    Cube& turnRightI();
};

#endif //CUBE_H
