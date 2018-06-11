/*
 * DynProg.hpp
 *
 *  Created on: May 15, 2018
 *      Author: Sarah Lutteropp
 */

#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <deque>

#include "dynamic_matrix.hpp"
#include "dist_config.hpp"
#include "matrix_entry.hpp"

class DynProg {
public:
	DynProg(const std::string& a, const std::string& b, const DistConfig& conf);
	int editDistance();
	double normalizedEditDistance();
	void addCharARight(char c);
	void addCharBRight(char c);
	void addCharALeft(char c);
	void addCharBLeft(char c);
	/*void removeCharARight();
	 void removeCharBRight();
	 void removeCharALeft(const DistConfig& conf);
	 void removeCharBLeft(const DistConfig& conf);*/
	const std::string& getA();
	const std::string& getB();
	void printUMatrix();
	void printLMatrix();
private:
	void dpIteration(int i, int j);
	void updateDrColwise();
	void updateDrRowwise();
	double normalizedEditDistance(int dist);
	DynamicMatrix<MatrixEntry> dr;
	std::string a;
	std::string b;
	DistConfig conf;

	double normEditDist_;
	int editDist_;
	bool distValid_;
};
