#include "cube.h"
#include "gtest/gtest.h"

class CubeTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		unsolvedCube = Cube(int* unsolvedCorners, int* unsolvedEdges);
	}

	Cube solvedCube;
	Cube unsolvedCube;
	
	int solvedCorners[8] = {1, 3, 5, 7, 13, 15, 17, 19};
	int solvedEdges[12] = {2, 4, 6, 8, 9, 10, 11, 12, 14, 16, 18, 20};
	
	int unsolvedCorners[8] = {17, 1, 13, 19, 5, 7, 15, 3};
	int unsolvedEdges[12] = {10, 6, 16, 9, 14, 8, 4, 11, 12, 18, 2, 20};
};

TEST_F(CubeTest, CheckMemberVariablesAfterConstruction) {
	EXPECT_TRUE(solvedCube.isSolved());
	EXPECT_FALSE(unsolvedCube.isSolved());
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(solvedCorners[i], solvedCube.get_corners()[i])
		EXPECT_EQ(unsolvedEdges[i], unsolvedCube.get_corners()[i])
	}
	for (int i = 0; i < 12; i++) {
		EXPECT_EQ(solvedEdges[i], solvedCube.get_edges()[i])
		EXPECT_EQ(unsolvedEdges[i], unsolvedCube.get_edges()[i])
	}
}