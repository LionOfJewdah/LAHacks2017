#ifndef CUBE_H
#define CUBE_H

class Cube {
public:
	Cube();
	Cube(int* edges, int* corners);
	bool isSolved();
	int* getEdges();
	int* getCorners();
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

private:
	bool solved;
	int* edges;
	int* corners;
};

#endif