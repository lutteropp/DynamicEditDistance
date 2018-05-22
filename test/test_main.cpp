/*
 * test_dynamic_matrix.cpp
 *
 *  Created on: May 16, 2018
 *      Author: Sarah Lutteropp
 */

#include <gtest/gtest.h>
#include <array>
#include <string>
#include <random>
#include <vector>

#include "../src/dynamic_matrix.hpp"
#include "../src/dyn_prog.hpp"
#include "../src/dist_config.hpp"

TEST(DynamicMatrix, addingColumnRight) {
	DynamicMatrix<int> mat;
	// 1 2 3
	// 4 5 6
	mat.setMinColIdx(0);
	mat.setMaxColIdx(2);
	mat.setMinRowIdx(0);
	mat.setMaxRowIdx(2);
	mat[0][0] = 1;
	mat[0][1] = 2;
	mat[0][2] = 3;
	mat[1][0] = 4;
	mat[1][1] = 5;
	mat[1][2] = 6;
	EXPECT_TRUE((mat[0][0] == 1));
	EXPECT_TRUE((mat[0][1] == 2));
	EXPECT_TRUE((mat[0][2] == 3));
	EXPECT_TRUE((mat[1][0] == 4));
	EXPECT_TRUE((mat[1][1] == 5));
	EXPECT_TRUE((mat[1][2] == 6));
	// 1 2 3 7
	// 4 5 6 8
	mat.setMaxColIdx(3);
	mat.setMaxRowIdx(3);
	mat[0][3] = 7;
	mat[1][3] = 8;
	EXPECT_TRUE((mat[0][0] == 1));
	EXPECT_TRUE((mat[0][1] == 2));
	EXPECT_TRUE((mat[0][2] == 3));
	EXPECT_TRUE((mat[1][0] == 4));
	EXPECT_TRUE((mat[1][1] == 5));
	EXPECT_TRUE((mat[1][2] == 6));
	EXPECT_TRUE((mat[0][3] == 7));
	EXPECT_TRUE((mat[1][3] == 8));
}

TEST(DynamicMatrix, addingColumnLeft) {
	DynamicMatrix<int> mat;
	// 1 2 3
	// 4 5 6
	mat.setMinColIdx(0);
	mat.setMaxColIdx(2);
	mat.setMinRowIdx(0);
	mat.setMaxRowIdx(2);
	mat[0][0] = 1;
	mat[0][1] = 2;
	mat[0][2] = 3;
	mat[1][0] = 4;
	mat[1][1] = 5;
	mat[1][2] = 6;
	mat.printMatrix();
	EXPECT_TRUE((mat[0][0] == 1));
	EXPECT_TRUE((mat[0][1] == 2));
	EXPECT_TRUE((mat[0][2] == 3));
	EXPECT_TRUE((mat[1][0] == 4));
	EXPECT_TRUE((mat[1][1] == 5));
	EXPECT_TRUE((mat[1][2] == 6));
	// 7 1 2 3
	// 8 4 5 6
	mat.setMinColIdx(-1);
	mat.setMinRowIdx(-1);
	mat[0][-1] = 7;
	mat[1][-1] = 8;
	mat.printMatrix();
	std::cout << "minColIdx: " << mat.getMinColIdx() << "\n";
	std::cout << "maxColIdx: " << mat.getMaxColIdx() << "\n";
	EXPECT_EQ(mat.getNCols(), 4);
	EXPECT_TRUE((mat[0][0] == 1));
	EXPECT_TRUE((mat[0][1] == 2));
	EXPECT_TRUE((mat[0][2] == 3));
	EXPECT_TRUE((mat[1][0] == 4));
	EXPECT_TRUE((mat[1][1] == 5));
	EXPECT_TRUE((mat[1][2] == 6));
	EXPECT_TRUE((mat[0][-1] == 7));
	EXPECT_TRUE((mat[1][-1] == 8));
}

char randomNuc() {
	std::array<char, 4> nucs = { 'A', 'C', 'G', 'T' };
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist4(0, 3);
	return nucs[dist4(rng)];
}

std::string randomDNA(size_t length) {
	std::string dna = "";
	for (size_t i = 0; i < length; ++i) {
		dna += randomNuc();
	}
	return dna;
}

int classicalEditDist(const std::string& s1, const std::string& s2, const DistConfig& config) {
	// create a DP table
	size_t nrows = s1.size() + 1;
	size_t ncols = s2.size() + 1;
	std::vector<std::vector<int> > matrix;
	for (size_t i = 0; i < nrows; ++i) {
		std::vector<int> row(ncols);
		matrix.push_back(row);
	}
	// initialize it
	for (size_t i = 0; i < nrows; ++i) {
		matrix[i][0] = i * config.deletion_penalty;
	}
	for (size_t j = 0; j < ncols; ++j) {
		matrix[0][j] = j * config.insertion_penalty;
	}
	// fill it
	for (size_t i = 1; i < nrows; ++i) {
		for (size_t j = 1; j < ncols; ++j) {
			int score = (s1[i - 1] == s2[j - 1]) ? 0 : config.substitution_penalty;
			matrix[i][j] = std::min(matrix[i - 1][j - 1] + score,
					std::min(matrix[i - 1][j] + config.deletion_penalty, matrix[i][j - 1] + config.insertion_penalty));
		}
	}
	return matrix[nrows - 1][ncols - 1];
}

TEST(DynProg, singleEqual) {
	DistConfig config;
	std::string s1 = "A";
	std::string s2 = "A";
	DynProg dp(s1, s2, config);
	EXPECT_EQ(dp.editDistance(config), 0);
}

TEST(DynProg, twoEqual) {
	DistConfig config;
	std::string s1 = "AG";
	std::string s2 = "AG";
	DynProg dp(s1, s2, config);
	EXPECT_EQ(dp.editDistance(config), 0);
}

TEST(DynProg, singleDifferent) {
	DistConfig config;
	std::string s1 = "A";
	std::string s2 = "G";
	DynProg dp(s1, s2, config);
	EXPECT_EQ(dp.editDistance(config), 1);
}

TEST(DynProg, twoDifferent) {
	DistConfig config;
	std::string s1 = "AG";
	std::string s2 = "TC";
	DynProg dp(s1, s2, config);
	EXPECT_EQ(dp.editDistance(config), 2);
}

TEST(DynProg, oneEqOneDiff) {
	DistConfig config;
	std::string s1 = "AG";
	std::string s2 = "TG";
	DynProg dp(s1, s2, config);
	EXPECT_EQ(dp.editDistance(config), 1);
}

TEST(DynProg, singleLeftAddEqual) {
	DistConfig config;
	std::string s1 = "";
	std::string s2 = "";
	DynProg dp(s1, s2, config);
	EXPECT_EQ(dp.editDistance(config), 0);
	s1 = "A" + s1;
	dp.addCharALeft('A', config);
	dp.printLMatrix();
	dp.printUMatrix();
	EXPECT_EQ(dp.editDistance(config), 1);
	s2 = "A" + s2;
	dp.addCharBLeft('A', config);
	dp.printLMatrix();
	dp.printUMatrix();
	EXPECT_EQ(dp.editDistance(config), 0);
}

TEST(DynProg, singleLeftAddDifferent) {
	DistConfig config;
	std::string s1 = "";
	std::string s2 = "";
	DynProg dp(s1, s2, config);
	EXPECT_EQ(dp.editDistance(config), 0);
	s1 = "A" + s1;
	dp.addCharALeft('A', config);
	EXPECT_EQ(dp.editDistance(config), 1);
	s2 = "G" + s2;
	dp.addCharBLeft('G', config);
	dp.printLMatrix();
	dp.printUMatrix();
	EXPECT_EQ(dp.editDistance(config), 1);
}

TEST(DynProg, problem) {
	DistConfig config;
	std::string s1 = "";
	std::string s2 = "";
	DynProg dp(s1, s2, config);
	dp.printLMatrix();
	dp.printUMatrix();
	EXPECT_EQ(dp.editDistance(config), 0);
	// C, -
	s1 = "C" + s1;
	dp.addCharALeft('C', config);
	dp.printLMatrix();
	dp.printUMatrix();
	EXPECT_EQ(dp.editDistance(config), 1);
	// GC, -
	s1 = "G" + s1;
	dp.addCharALeft('G', config);
	dp.printLMatrix();
	dp.printUMatrix();
	EXPECT_EQ(dp.editDistance(config), 2);
	// AGC, -
	s1 = "A" + s1;
	dp.addCharALeft('A', config);
	dp.printLMatrix();
	dp.printUMatrix();
	EXPECT_EQ(dp.editDistance(config), 3);
	// AGC, C
	s2 = "C" + s2;
	dp.addCharBLeft('C', config);
	dp.printLMatrix();
	dp.printUMatrix();
	EXPECT_EQ(dp.editDistance(config), 2);
}

TEST(DynProg, twoSidedAdd) {
	DistConfig config;
	std::string s1 = "";
	std::string s2 = "";
	DynProg dp(s1, s2, config);
	s1 = "C" + s1;
	dp.addCharALeft('C', config);
	dp.printLMatrix();
	dp.printUMatrix();
	s1 = "T" + s1;
	dp.addCharALeft('T', config);
	dp.printLMatrix();
	dp.printUMatrix();
	s2 = s2 + "T";
	dp.addCharBRight('T', config);
	dp.printLMatrix();
	dp.printUMatrix();
	EXPECT_EQ(dp.editDistance(config), 1);
}

TEST(DynProg, random) {
	DistConfig config;
	std::string s1 = "";
	std::string s2 = "";
	DynProg dp(s1, s2, config);
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist4(0, 3);
	std::cout << "s1: " << s1 << "\n";
	std::cout << "s2: " << s2 << "\n";
	EXPECT_EQ(dp.editDistance(config), classicalEditDist(s1, s2, config));
	for (size_t i = 0; i < 200; ++i) {
		int rand = dist4(rng);
		if (rand == 0) {
			std::cout << "right add\n";
			s1 = s1 + randomNuc();
			dp.addCharARight(s1[s1.size() - 1], config);
		} else if (rand == 1) {
			std::cout << "right add\n";
			s2 = s2 + randomNuc();
			dp.addCharBRight(s2[s2.size() - 1], config);
		} else if (rand == 2) {
			std::cout << "left add\n";
			s1 = randomDNA(1) + s1;
			dp.addCharALeft(s1[0], config);
		} else {
			std::cout << "left add\n";
			s2 = randomDNA(1) + s2;
			dp.addCharBLeft(s2[0], config);
		}
		std::cout << "s1: " << s1 << "\n";
		std::cout << "s2: " << s2 << "\n";
		EXPECT_EQ(dp.editDistance(config), classicalEditDist(s1, s2, config));

		if (dp.editDistance(config) != classicalEditDist(s1, s2, config)) {
			break;
		}
	}
}

TEST(DynProg, weightedDebug) {
	DistConfig config(1, 2, 3);
	DynProg dpTest("CA", "T", config);
	dpTest.printLMatrix();
	dpTest.printUMatrix();
	dpTest.editDistance(config);
	EXPECT_EQ(dpTest.editDistance(config), classicalEditDist("CA", "T", config));
}

TEST(DynProg, randomWeighted) {
	DistConfig config(1, 2, 3);
	std::string s1 = "";
	std::string s2 = "";
	DynProg dp(s1, s2, config);
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist4(0, 3);
	std::cout << "s1: " << s1 << "\n";
	std::cout << "s2: " << s2 << "\n";
	EXPECT_EQ(dp.editDistance(config), classicalEditDist(s1, s2, config));
	if (dp.editDistance(config) != classicalEditDist(s1, s2, config)) {
		return;
	}
	for (size_t i = 0; i < 200; ++i) {
		int rand = dist4(rng);
		if (rand == 0) {
			std::cout << "right add\n";
			s1 = s1 + randomNuc();
			dp.addCharARight(s1[s1.size() - 1], config);
		} else if (rand == 1) {
			std::cout << "right add\n";
			s2 = s2 + randomNuc();
			dp.addCharBRight(s2[s2.size() - 1], config);
		} else if (rand == 2) {
			std::cout << "left add\n";
			s1 = randomDNA(1) + s1;
			dp.addCharALeft(s1[0], config);
		} else {
			std::cout << "left add\n";
			s2 = randomDNA(1) + s2;
			dp.addCharBLeft(s2[0], config);
		}
		std::cout << "s1: " << s1 << "\n";
		std::cout << "s2: " << s2 << "\n";
		EXPECT_EQ(dp.editDistance(config), classicalEditDist(s1, s2, config));
		if (dp.editDistance(config) != classicalEditDist(s1, s2, config)) {
			break;
		}
	}
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
