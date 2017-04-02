#include "cube.h"
#include "gtest/gtest.h"

class CubeTest : public ::testing::Test {
protected:
	CubeTest() : unsolvedCube(unsolvedCorners, unsolvedEdges) {

	}

	virtual void SetUp() {
	}

	Cube solvedCube;
	Cube unsolvedCube;
	
	const int solvedCorners[8] = {1, 3, 5, 7, 13, 15, 17, 19};
	const int solvedEdges[12] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
	
	const int unsolvedCorners[8] = {17, 1, 13, 19, 5, 7, 15, 3};
	const int unsolvedEdges[12] = {10, 6, 16, 9, 14, 8, 4, 11, 12, 18, 2, 20};
};

TEST_F(CubeTest, CheckMemberVariablesAfterConstruction) {
	EXPECT_TRUE(solvedCube.isSolved());
	EXPECT_FALSE(unsolvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(solvedCorners[i], solvedCube.get_corners()[i]);
		EXPECT_EQ(unsolvedEdges[i], unsolvedCube.get_corners()[i]);
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
	int newCorners[] = {1, 3, 5, 7, 13, 15, 17, 19};
	int newEdges[] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
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
	
	int newCorners2[] = {1, 3, 5, 7, 13, 15, 17, 19};
	int newEdges2[] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
	
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
	
	int newCorners3[] = {1, 3, 5, 7, 13, 15, 17, 19};
	int newEdges3[] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
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
	int newCorners[] = {1, 3, 5, 7, 13, 15, 17, 19};
	int newEdges[] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
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
	
	int newCorners2[] = {1, 3, 5, 7, 13, 15, 17, 19};
	int newEdges2[] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
	
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
	
	int newCorners3[] = {1, 3, 5, 7, 13, 15, 17, 19};
	int newEdges3[] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
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
	int newCorners[] = {1, 3, 5, 7, 13, 15, 17, 19};
	int newEdges[] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
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
	
	int newCorners2[] = {1, 3, 5, 7, 13, 15, 17, 19};
	int newEdges2[] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
	
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
	
	int newCorners3[] = {1, 3, 5, 7, 13, 15, 17, 19};
	int newEdges3[] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
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
	int newCorners[] = {1, 3, 5, 7, 13, 15, 17, 19};
	int newEdges[] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
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
	
	int newCorners2[] = {1, 3, 5, 7, 13, 15, 17, 19};
	int newEdges2[] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
	
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
	
	int newCorners3[] = {1, 3, 5, 7, 13, 15, 17, 19};
	int newEdges3[] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(newCorners3[i], solvedCube.get_corners()[i]);
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(newEdges3[i], solvedCube.get_edges()[i]);
	}
	
	solvedCube.turnRight2();
	EXPECT_TRUE(solvedCube.isSolved());
}































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