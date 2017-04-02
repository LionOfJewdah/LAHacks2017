#include "cube.h"
#include "gtest/gtest.h"

#include <iostream>
using namespace std;

class CubeTest : public ::testing::Test {
protected:
	const int solvedCorners[8] = {1, 3, 5, 7, 13, 15, 17, 19};
	const int solvedEdges[12] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
	

	CubeTest() : unsolvedCube(unsolvedCorners, unsolvedEdges) {

	}

	virtual void SetUp() {
	}

	Cube solvedCube;
	Cube unsolvedCube;
public:
	constexpr static int 
		unsolvedCorners[8] = {17, 1, 13, 19, 5, 7, 15, 3};
	constexpr static int 
		unsolvedEdges[12] = {10, 6, 16, 9, 14, 8, 4, 11, 12, 18, 2, 20};
};
constexpr int CubeTest::
	unsolvedCorners[8];// = {17, 1, 13, 19, 5, 7, 15, 3};
constexpr int CubeTest:: 
	unsolvedEdges[12];// = {10, 6, 16, 9, 14, 8, 4, 11, 12, 18, 2, 20};


TEST_F(CubeTest, CheckMemberVariablesAfterConstruction) {
	EXPECT_TRUE(solvedCube.isSolved());
	EXPECT_FALSE(unsolvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(solvedCorners[i], solvedCube.get_corners()[i]);
		EXPECT_EQ(unsolvedCorners[i], unsolvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(solvedEdges[i], solvedCube.get_edges()[i]);
		EXPECT_EQ(unsolvedEdges[i], unsolvedCube.get_edges()[i]);
	}
}

TEST_F(CubeTest, CheckAllUpFunctions) {
	solvedCube.turnUp();
	int newCorners[] = {3, 5, 7, 1, 13, 15, 17, 19};
	int newEdges[] = {4, 6, 8, 2, 9, 10, 11, 12, 14, 16, 18, 20};
	EXPECT_FALSE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges[i], solvedCube.get_edges()[i]);
	}
	
	solvedCube.turnUpI();
	EXPECT_TRUE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(solvedCorners[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(solvedEdges[i], solvedCube.get_edges()[i]);
	}
	
	int newCorners2[] = {7, 1, 3, 5, 13, 15, 17, 19};
	int newEdges2[] = {8, 2, 4, 6, 9, 10, 11, 12, 14, 16, 18, 20};
	
	solvedCube.turnUpI();
	EXPECT_FALSE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners2[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges2[i], solvedCube.get_edges()[i]);
	}

	solvedCube.turnUp();
	EXPECT_TRUE(solvedCube.isSolved());
	
	solvedCube.turnUp2();
	EXPECT_FALSE(solvedCube.isSolved());
	
	int newCorners3[] = {5, 7, 1, 3, 13, 15, 17, 19};
	int newEdges3[] = {6, 8, 2, 4, 9, 10, 11, 12, 14, 16, 18, 20};
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners3[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges3[i], solvedCube.get_edges()[i]);
	}
	
	solvedCube.turnUp2();
	EXPECT_TRUE(solvedCube.isSolved());
}

TEST_F(CubeTest, CheckAllDownFunctions) {
	solvedCube.turnDown();
	int newCorners[] = {1, 3, 5, 7, 19, 13, 15, 17};
	int newEdges[] = {2, 4, 6, 8, 9, 10, 11, 12, 20, 14, 16, 18};
	EXPECT_FALSE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges[i], solvedCube.get_edges()[i]);
	}
	
	solvedCube.turnDownI();
	EXPECT_TRUE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(solvedCorners[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(solvedEdges[i], solvedCube.get_edges()[i]);
	}
	
	int newCorners2[] = {1, 3, 5, 7, 15, 17, 19, 13};
	int newEdges2[] = {2, 4, 6, 8, 9, 10, 11, 12, 16, 18, 20, 14};
	
	solvedCube.turnDownI();
	EXPECT_FALSE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners2[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges2[i], solvedCube.get_edges()[i]);
	}

	solvedCube.turnDown();
	EXPECT_TRUE(solvedCube.isSolved());
	
	solvedCube.turnDown2();
	EXPECT_FALSE(solvedCube.isSolved());
	
	int newCorners3[] = {1, 3, 5, 7, 17, 19, 13, 15};
	int newEdges3[] = {2, 4, 6, 8, 9, 10, 11, 12, 18, 20, 14, 16};
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners3[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges3[i], solvedCube.get_edges()[i]);
	}
	
	solvedCube.turnDown2();
	EXPECT_TRUE(solvedCube.isSolved());
}

TEST_F(CubeTest, CheckAllFrontFunctions) {
	solvedCube.turnFront();
	int newCorners[] = {13, 1, 5, 7, 15, 3, 17, 19};
	int newEdges[] = {9, 4, 6, 8, 14, 2, 11, 12, 10, 16, 18, 20};
	EXPECT_FALSE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges[i], solvedCube.get_edges()[i]);
	}
	
	solvedCube.turnFrontI();
	EXPECT_TRUE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(solvedCorners[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(solvedEdges[i], solvedCube.get_edges()[i]);
	}
	
	int newCorners2[] = {3, 15, 5, 7, 1, 13, 17, 19};
	int newEdges2[] = {10, 4, 6, 8, 2, 14, 11, 12, 9, 16, 18, 20};
	
	solvedCube.turnFrontI();
	EXPECT_FALSE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners2[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges2[i], solvedCube.get_edges()[i]);
	}

	solvedCube.turnFront();
	EXPECT_TRUE(solvedCube.isSolved());
	
	solvedCube.turnFront2();
	EXPECT_FALSE(solvedCube.isSolved());
	
	int newCorners3[] = {15, 13, 5, 7, 3, 1, 17, 19};
	int newEdges3[] = {14, 4, 6, 8, 10, 9, 11, 12, 2, 16, 18, 20};
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners3[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges3[i], solvedCube.get_edges()[i]);
	}
	
	solvedCube.turnFront2();
	EXPECT_TRUE(solvedCube.isSolved());
}

TEST_F(CubeTest, CheckAllBackFunctions) {
	solvedCube.turnBack();
	int newCorners[] = {1, 3, 17, 5, 13, 15, 19, 7};
	int newEdges[] = {2, 4, 11, 8, 9, 10, 18, 6, 14, 16, 12, 20};
	EXPECT_FALSE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges[i], solvedCube.get_edges()[i]);
	}
	
	solvedCube.turnBackI();
	EXPECT_TRUE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(solvedCorners[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(solvedEdges[i], solvedCube.get_edges()[i]);
	}
	
	int newCorners2[] = {1, 3, 7, 19, 13, 15, 5, 17};
	int newEdges2[] = {2, 4, 12, 8, 9, 10, 6, 18, 14, 16, 11, 20};
	
	solvedCube.turnBackI();
	EXPECT_FALSE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners2[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges2[i], solvedCube.get_edges()[i]);
	}

	solvedCube.turnBack();
	EXPECT_TRUE(solvedCube.isSolved());
	
	solvedCube.turnBack2();
	EXPECT_FALSE(solvedCube.isSolved());
	
	int newCorners3[] = {1, 3, 19, 17, 13, 15, 7, 5};
	int newEdges3[] = {2, 4, 18, 8, 9, 10, 12, 11, 14, 16, 6, 20};
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners3[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges3[i], solvedCube.get_edges()[i]);
	}
	
	solvedCube.turnBack2();
	EXPECT_TRUE(solvedCube.isSolved());
}

TEST_F(CubeTest, CheckAllLeftFunctions) {
	solvedCube.turnLeft();
	int newCorners[] = {7, 3, 5, 19, 1, 15, 17, 13};
	int newEdges[] = {2, 4, 6, 12, 8, 10, 11, 20, 14, 16, 18, 9};
	EXPECT_FALSE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges[i], solvedCube.get_edges()[i]);
	}
	
	solvedCube.turnLeftI();
	EXPECT_TRUE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(solvedCorners[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(solvedEdges[i], solvedCube.get_edges()[i]);
	}
	
	int newCorners2[] = {13, 3, 5, 1, 19, 15, 17, 7};
	int newEdges2[] = {2, 4, 6, 9, 20, 10, 11, 8, 14, 16, 18, 12};
	
	solvedCube.turnLeftI();
	EXPECT_FALSE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners2[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges2[i], solvedCube.get_edges()[i]);
	}

	solvedCube.turnLeft();
	EXPECT_TRUE(solvedCube.isSolved());
	
	solvedCube.turnLeft2();
	EXPECT_FALSE(solvedCube.isSolved());
	
	int newCorners3[] = {19, 3, 5, 13, 7, 15, 17, 1};
	int newEdges3[] = {2, 4, 6, 20, 12, 10, 11, 9, 14, 16, 18, 8};
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners3[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges3[i], solvedCube.get_edges()[i]);
	}
	
	solvedCube.turnLeft2();
	EXPECT_TRUE(solvedCube.isSolved());
}

TEST_F(CubeTest, CheckAllRightFunctions) {
	solvedCube.turnRight();
	int newCorners[] = {1, 15, 3, 7, 13, 17, 5, 19};
	int newEdges[] = {2, 10, 6, 8, 9, 16, 4, 12, 14, 11, 18, 20};
	EXPECT_FALSE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges[i], solvedCube.get_edges()[i]);
	}
	
	solvedCube.turnRightI();
	EXPECT_TRUE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(solvedCorners[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(solvedEdges[i], solvedCube.get_edges()[i]);
	}
	
	int newCorners2[] = {1, 5, 17, 7, 13, 3, 15, 19};
	int newEdges2[] = {2, 11, 6, 8, 9, 4, 16, 12, 14, 10, 18, 20};
	
	solvedCube.turnRightI();
	EXPECT_FALSE(solvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners2[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges2[i], solvedCube.get_edges()[i]);
	}

	solvedCube.turnRight();
	EXPECT_TRUE(solvedCube.isSolved());
	
	solvedCube.turnRight2();
	EXPECT_FALSE(solvedCube.isSolved());
	
	int newCorners3[] = {1, 17, 15, 7, 13, 5, 3, 19};
	int newEdges3[] = {2, 16, 6, 8, 9, 11, 10, 12, 14, 4, 18, 20};
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners3[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges3[i], solvedCube.get_edges()[i]);
	}
	
	solvedCube.turnRight2();
	EXPECT_TRUE(solvedCube.isSolved());
}






























/*
TEST_F(CubeTest, CheckMoveThenI) {
}

TEST_F(CubeTest, CheckIThenMove) {
}

TEST_F(CubeTest, CheckMoveTwo) {
}

TEST_F(CubeTest, CheckMove360Quarter) {
}

TEST_F(CubeTest, CheckMoveInverse360Quarter) {
}

TEST_F(CubeTest, CheckMove360Half) {
}
*/