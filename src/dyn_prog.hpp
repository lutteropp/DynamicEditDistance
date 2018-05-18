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
	int editDistance(const DistConfig& conf);
	void addCharARight(char c, const DistConfig& conf);
	void addCharBRight(char c, const DistConfig& conf);
	void removeCharARight();
	void removeCharBRight();
	void addCharALeft(char c, const DistConfig& conf);
	void addCharBLeft(char c, const DistConfig& conf);
	void removeCharALeft(const DistConfig& conf);
	void removeCharBLeft(const DistConfig& conf);
	const std::string& getA();
	const std::string& getB();
private:
	void dpIteration(int i, int j, const DistConfig& conf);
	void updateDrColwise(const DistConfig& conf);
	void updateDrRowwise(const DistConfig& conf);
	DynamicMatrix<MatrixEntry> dr;
	int m;
	int n;
	std::string a;
	std::string b;
	int rowShift;
	int colShift;
};

void DynProg::dpIteration(int i, int j, const DistConfig& conf) {
	int sub = (a[i] == b[j] ? 0 : conf.substitution_penalty);
	int z = std::min(
			std::min(dr[ { i - 1, j }].l + conf.insertion_penalty, dr[ { i, j - 1 }].u + conf.deletion_penalty), sub);
	dr[ { i, j }].u = z - dr[ { i - 1, j }].l;
	dr[ { i, j }].l = z - dr[ { i, j - 1 }].u;
}

DynProg::DynProg(const std::string& a, const std::string& b, const DistConfig& conf) {
	// compute the initial delta values...
	m = a.size();
	n = b.size();
	for (int i = 1; i <= m; ++i) {
		dr[ { i, 0 }].u = conf.deletion_penalty;
	}
	for (int j = 1; j <= n; ++j) {
		dr[ { 0, j }].l = conf.insertion_penalty;
	}
	dr.printMatrix();
	for (int i = 1; i <= m; ++i) {
		for (int j = 1; j <= n; ++j) {
			dpIteration(i, j, conf);
		}
	}
	dr.printMatrix();
	this->a = a;
	this->b = b;
	rowShift = 0;
	colShift = 0;
}

int DynProg::editDistance(const DistConfig& conf) {
	int e = 0;
	if (m <= n) { // iterate over i in D[i,n]
		e = n * conf.insertion_penalty;
		for (int i = 2; i <= m; ++i) {
			e += dr[ { i - 1, n }].u;
		}
	} else { // iterate over j in D[m,j]
		e = m * conf.deletion_penalty;
		for (int j = 2; j <= n; ++j) {
			e += dr[ { m, j - 1 }].l;
		}
	}
	return e;
}

void DynProg::addCharARight(char c, const DistConfig& conf) {
	a += c;
	m++;
	dr[ { m, 0 }].u = conf.deletion_penalty;
	for (int j = 1; j <= n; ++j) {
		dpIteration(m, j, conf);
	}
}

void DynProg::addCharBRight(char c, const DistConfig& conf) {
	b += c;
	n++;
	dr[ { 0, n }].l = conf.insertion_penalty;
	for (int i = 1; i <= m; ++i) {
		dpIteration(i, n, conf);
	}
}

void DynProg::removeCharARight() {
	m--;
	a = a.substr(0, m);
}

void DynProg::removeCharBRight() {
	n--;
	b = b.substr(0, n);
}

const std::string& DynProg::getA() {
	return a;
}

const std::string& DynProg::getB() {
	return b;
}

void DynProg::updateDrColwise(const DistConfig& conf) {
	std::deque<int> prevChanged;
	// new first column now
	for (int i = 1; i <= m; ++i) {
		dr[ { i, 0 }].u = conf.deletion_penalty;
		prevChanged.push_back(i);
	}

	// go column by column
	for (int j = 1; j <= n; ++j) {
		if (prevChanged.empty()) {
			break;
		}

		std::deque<int> currChanged;
		while (!prevChanged.empty()) {
			int i = prevChanged.front();
			prevChanged.pop_front();

			// recompute values
			int oldU = dr[ { i, j }].u;
			int oldL = dr[ { i, j }].l;

			int sub = (a[i] == b[j] ? 0 : conf.substitution_penalty);
			int z = std::min(
					std::min(dr[ { i - 1, j }].l + conf.insertion_penalty, dr[ { i, j - 1 }].u + conf.deletion_penalty),
					sub);
			int newU = z - dr[ { i - 1, j }].l;
			int newL = z - dr[ { i, j - 1 }].u;
			dr[ { i, j }].u = newU;
			dr[ { i, j }].l = newL;
			if (oldU != newU) {
				currChanged.push_back(i);
			}

			if (oldL != newL) {
				if (prevChanged.empty() || prevChanged.front() != i + 1) {
					if (i + 1 <= m) {
						prevChanged.push_front(i + 1);
					}
				}
			}
		}
		prevChanged = currChanged;
	}
}

void DynProg::updateDrRowwise(const DistConfig& conf) {
	std::deque<int> prevChanged;
	// new first row now
	for (int j = 1; j <= n; ++j) {
		dr[ { 0, j }].l = conf.insertion_penalty;
		prevChanged.push_back(j);
	}

	//go row by row
	for (int i = 1; i <= m; ++i) {
		if (prevChanged.empty()) {
			break;
		}

		std::deque<int> currChanged;
		while (!prevChanged.empty()) {
			int j = prevChanged.front();
			prevChanged.pop_front();

			// recompute values
			int oldU = dr[ { i, j }].u;
			int oldL = dr[ { i, j }].l;

			int sub = (a[i] == b[j] ? 0 : conf.substitution_penalty);
			int z = std::min(
					std::min(dr[ { i - 1, j }].l + conf.insertion_penalty, dr[ { i, j - 1 }].u + conf.deletion_penalty),
					sub);
			int newU = z - dr[ { i - 1, j }].l;
			int newL = z - dr[ { i, j - 1 }].u;
			dr[ { i, j }].u = newU;
			dr[ { i, j }].l = newL;
			if (oldL != newL) {
				currChanged.push_back(j);
			}

			if (oldU != newU) {
				if (prevChanged.empty() || prevChanged.front() != j + 1) {
					if (j + 1 <= n) {
						prevChanged.push_front(j + 1);
					}
				}
			}
		}
		prevChanged = currChanged;
	}
}

void DynProg::addCharALeft(char c, const DistConfig& conf) {
	std::string aNew = "";
	aNew += c;
	a = aNew + a;
	m++;
	rowShift++;
	dr.setExternalRowOffset(rowShift);
	updateDrRowwise(conf);
}

void DynProg::addCharBLeft(char c, const DistConfig& conf) {
	std::string bNew = "";
	bNew += c;
	b = bNew + b;
	n++;
	colShift++;
	dr.setExternalColOffset(colShift);
	updateDrColwise(conf);
}

void DynProg::removeCharALeft(const DistConfig& conf) {
	m--;
	a = a.substr(1, m);
	rowShift--;
	dr.setExternalRowOffset(rowShift);
	updateDrRowwise(conf);
}

void DynProg::removeCharBLeft(const DistConfig& conf) {
	n--;
	b = b.substr(1, n);
	colShift--;
	dr.setExternalColOffset(colShift);
	updateDrColwise(conf);
}
