/*
 * test_dynamic_matrix.cpp
 *
 *  Created on: May 16, 2018
 *      Author: Sarah Lutteropp
 */

#include <gtest/gtest.h>
#include <array>

#include "../src/dynamic_matrix.hpp"

TEST(DynamicMatrix, addingColumnRight) {
	DynamicMatrix<int> mat;
	// 1 2 3
	// 4 5 6
	mat[ { 0, 0 }] = 1;
	mat[ { 0, 1 }] = 2;
	mat[ { 0, 2 }] = 3;
	mat[ { 1, 0 }] = 4;
	mat[ { 1, 1 }] = 5;
	mat[ { 1, 2 }] = 6;
	EXPECT_TRUE((mat[ { 0, 0 }] == 1));
	EXPECT_TRUE((mat[ { 0, 1 }] == 2));
	EXPECT_TRUE((mat[ { 0, 2 }] == 3));
	EXPECT_TRUE((mat[ { 1, 0 }] == 4));
	EXPECT_TRUE((mat[ { 1, 1 }] == 5));
	EXPECT_TRUE((mat[ { 1, 2 }] == 6));
	// 1 2 3 7
	// 4 5 6 8
	mat[ { 0, 3 }] = 7;
	mat[ { 1, 3 }] = 8;
	EXPECT_TRUE((mat[ { 0, 0 }] == 1));
	EXPECT_TRUE((mat[ { 0, 1 }] == 2));
	EXPECT_TRUE((mat[ { 0, 2 }] == 3));
	EXPECT_TRUE((mat[ { 1, 0 }] == 4));
	EXPECT_TRUE((mat[ { 1, 1 }] == 5));
	EXPECT_TRUE((mat[ { 1, 2 }] == 6));
	EXPECT_TRUE((mat[ { 0, 3 }] == 7));
	EXPECT_TRUE((mat[ { 1, 3 }] == 8));
}

TEST(DynamicMatrix, addingColumnLeft) {
	DynamicMatrix<int> mat;
	// 1 2 3
	// 4 5 6
	mat[ { 0, 0 }] = 1;
	mat[ { 0, 1 }] = 2;
	mat[ { 0, 2 }] = 3;
	mat[ { 1, 0 }] = 4;
	mat[ { 1, 1 }] = 5;
	mat[ { 1, 2 }] = 6;
	mat.printMatrix();
	EXPECT_TRUE((mat[ { 0, 0 }] == 1));
	EXPECT_TRUE((mat[ { 0, 1 }] == 2));
	EXPECT_TRUE((mat[ { 0, 2 }] == 3));
	EXPECT_TRUE((mat[ { 1, 0 }] == 4));
	EXPECT_TRUE((mat[ { 1, 1 }] == 5));
	EXPECT_TRUE((mat[ { 1, 2 }] == 6));
	// 7 1 2 3
	// 8 4 5 6
	mat[ { 0, -1 }] = 7;
	mat[ { 1, -1 }] = 8;
	mat.printMatrix();
	EXPECT_EQ(mat.getNCols(), 4);
	EXPECT_TRUE((mat[ { 0, 0 }] == 1));
	EXPECT_TRUE((mat[ { 0, 1 }] == 2));
	EXPECT_TRUE((mat[ { 0, 2 }] == 3));
	EXPECT_TRUE((mat[ { 1, 0 }] == 4));
	EXPECT_TRUE((mat[ { 1, 1 }] == 5));
	EXPECT_TRUE((mat[ { 1, 2 }] == 6));
	EXPECT_TRUE((mat[ { 0, -1 }] == 7));
	EXPECT_TRUE((mat[ { 1, -1 }] == 8));
}

TEST(DynamicMatrix, addingColumnLeftShifting) {
	DynamicMatrix<int> mat;
	// 1 2 3
	// 4 5 6
	mat[ { 0, 0 }] = 1;
	mat[ { 0, 1 }] = 2;
	mat[ { 0, 2 }] = 3;
	mat[ { 1, 0 }] = 4;
	mat[ { 1, 1 }] = 5;
	mat[ { 1, 2 }] = 6;
	mat.printMatrix();
	EXPECT_TRUE((mat[ { 0, 0 }] == 1));
	EXPECT_TRUE((mat[ { 0, 1 }] == 2));
	EXPECT_TRUE((mat[ { 0, 2 }] == 3));
	EXPECT_TRUE((mat[ { 1, 0 }] == 4));
	EXPECT_TRUE((mat[ { 1, 1 }] == 5));
	EXPECT_TRUE((mat[ { 1, 2 }] == 6));
	// 7 1 2 3
	// 8 4 5 6
	mat.setExternalColOffset(1);
	mat[ { 0, 0 }] = 7;
	mat[ { 1, 0 }] = 8;
	mat.printMatrix();
	EXPECT_TRUE((mat[ { 0, 1 }] == 1));
	EXPECT_TRUE((mat[ { 0, 2 }] == 2));
	EXPECT_TRUE((mat[ { 0, 3 }] == 3));
	EXPECT_TRUE((mat[ { 1, 1 }] == 4));
	EXPECT_TRUE((mat[ { 1, 2 }] == 5));
	EXPECT_TRUE((mat[ { 1, 3 }] == 6));
	EXPECT_TRUE((mat[ { 0, 0 }] == 7));
	EXPECT_TRUE((mat[ { 1, 0 }] == 8));
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
