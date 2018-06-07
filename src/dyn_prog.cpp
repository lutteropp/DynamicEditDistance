/*
 * dyn_prog.cpp
 *
 *  Created on: Jun 7, 2018
 *      Author: Sarah Lutteropp
 */

#include "dyn_prog.hpp"

void DynProg::dpIteration(int i, int j) {
	int sub = (a[i - 1 - dr.getMinRowIdx()] == b[j - 1 - dr.getMinColIdx()] ? 0 : conf.substitution_penalty);
	int z = std::min(std::min(dr[i - 1][j].l + conf.deletion_penalty, dr[i][j - 1].u + conf.insertion_penalty), sub);
	dr[i][j].u = z - dr[i - 1][j].l;
	dr[i][j].l = z - dr[i][j - 1].u;
}

DynProg::DynProg(const std::string& a, const std::string& b, const DistConfig& conf) {
	// compute the initial delta values...
	this->a = a;
	this->b = b;
	this->conf = conf;
	dr.setMinRowIdx(0);
	dr.setMinColIdx(0);
	dr.setMaxRowIdx(a.size());
	dr.setMaxColIdx(b.size());
	for (int i = dr.getMinRowIdx() + 1; i <= dr.getMaxRowIdx(); ++i) {
		dr[i][dr.getMinColIdx()].u = conf.deletion_penalty;
	}
	for (int j = dr.getMinColIdx() + 1; j <= dr.getMaxColIdx(); ++j) {
		dr[dr.getMinRowIdx()][j].l = conf.insertion_penalty;
	}
	for (int i = dr.getMinRowIdx() + 1; i <= dr.getMaxRowIdx(); ++i) {
		for (int j = dr.getMinColIdx() + 1; j <= dr.getMaxColIdx(); ++j) {
			dpIteration(i, j);
		}
	}
}

int DynProg::editDistance() {
	// corner case: one of the strings is empty
	if (a.size() == 0 && b.size() == 0) {
		return 0;
	}
	if (a.size() == 0 && b.size() > 0) {
		return b.size() * conf.insertion_penalty;
	}
	if (a.size() > 0 && b.size() == 0) {
		return a.size() * conf.deletion_penalty;
	}

	int e = 0;
	if (dr.getNRows() <= dr.getNCols()) { // iterate over i in D[i,n]
		e = (dr.getNCols() - 1) * conf.insertion_penalty;
		for (int i = dr.getMinRowIdx() + 1; i <= dr.getMaxRowIdx(); ++i) {
			e += dr[i][dr.getMaxColIdx()].u;
		}
	} else { // iterate over j in D[m,j]
		e = (dr.getNRows() - 1) * conf.deletion_penalty;
		for (int j = dr.getMinColIdx() + 1; j <= dr.getMaxColIdx(); ++j) {
			e += dr[dr.getMaxRowIdx()][j].l;
		}
	}
	return e;
}

void DynProg::addCharARight(char c) {
	a += c;
	dr.setMaxRowIdx(dr.getMaxRowIdx() + 1);
	dr[dr.getMaxRowIdx()][dr.getMinColIdx()].u = conf.deletion_penalty;
	for (int j = dr.getMinColIdx() + 1; j <= dr.getMaxColIdx(); ++j) {
		dpIteration(dr.getMaxRowIdx(), j);
	}
}

void DynProg::addCharBRight(char c) {
	b += c;
	dr.setMaxColIdx(dr.getMaxColIdx() + 1);
	dr[dr.getMinRowIdx()][dr.getMaxColIdx()].l = conf.insertion_penalty;
	for (int i = dr.getMinRowIdx() + 1; i <= dr.getMaxRowIdx(); ++i) {
		dpIteration(i, dr.getMaxColIdx());
	}
}

const std::string& DynProg::getA() {
	return a;
}

const std::string& DynProg::getB() {
	return b;
}

void DynProg::updateDrColwise() {
	std::deque<int> prevChanged;
	// new first column now
	for (int i = dr.getMinRowIdx() + 1; i <= dr.getMaxRowIdx(); ++i) {
		dr[i][dr.getMinColIdx()].u = conf.deletion_penalty;
		prevChanged.push_back(i);
	}
	// set the missing L value
	dr[dr.getMinRowIdx()][dr.getMinColIdx() + 1].l = conf.insertion_penalty;

	// go column by column
	for (int j = dr.getMinColIdx() + 1; j <= dr.getMaxColIdx(); ++j) {
		if (prevChanged.empty()) {
			break;
		}

		std::deque<int> currChanged;
		while (!prevChanged.empty()) {
			int i = prevChanged.front();
			prevChanged.pop_front();

			// recompute values
			int oldU = dr[i][j].u;
			int oldL = dr[i][j].l;

			int sub = (a[i - 1 - dr.getMinRowIdx()] == b[j - 1 - dr.getMinColIdx()] ? 0 : conf.substitution_penalty);
			int z = std::min(
					std::min(dr[i-1][j].l + conf.deletion_penalty, dr[i][j-1].u + conf.insertion_penalty),
					sub);
			int newU = z - dr[i-1][j].l;
			int newL = z - dr[i][j-1].u;
			dr[i][j].u = newU;
			dr[i][j].l = newL;
			if (oldU != newU) {
				currChanged.push_back(i);
			}

			if (oldL != newL) {
				if (prevChanged.empty() || prevChanged.front() != i + 1) {
					if (i + 1 <= dr.getMaxRowIdx()) {
						prevChanged.push_front(i + 1);
					}
				}
			}
		}
		prevChanged = currChanged;
	}
}

void DynProg::updateDrRowwise() {
	std::deque<int> prevChanged;
	// new first row now
	for (int j = 1 + dr.getMinColIdx(); j <= dr.getMaxColIdx(); ++j) {
		dr[dr.getMinRowIdx()][j].l = conf.insertion_penalty;
		prevChanged.push_back(j);
	}

	// set the missing U value
	dr[dr.getMinRowIdx() + 1][dr.getMinColIdx()].u = conf.deletion_penalty;

	//go row by row
	for (int i = 1 + dr.getMinRowIdx(); i <= dr.getMaxRowIdx(); ++i) {
		if (prevChanged.empty()) {
			break;
		}

		std::deque<int> currChanged;
		while (!prevChanged.empty()) {
			int j = prevChanged.front();
			prevChanged.pop_front();

			// recompute values
			int oldU = dr[i][j].u;
			int oldL = dr[i][j].l;

			int sub = (a[i - 1 - dr.getMinRowIdx()] == b[j - 1 - dr.getMinColIdx()] ? 0 : conf.substitution_penalty);
			int z = std::min(
					std::min(dr[i-1][j].l + conf.deletion_penalty, dr[i][j-1].u + conf.insertion_penalty),
					sub);
			int newU = z - dr[i-1][j].l;
			int newL = z - dr[i][j-1].u;
			dr[i][j].u = newU;
			dr[i][j].l = newL;
			if (oldL != newL) {
				currChanged.push_back(j);
			}

			if (oldU != newU) {
				if (prevChanged.empty() || prevChanged.front() != j + 1) {
					if (j + 1 <= dr.getMaxColIdx()) {
						prevChanged.push_front(j + 1);
					}
				}
			}
		}
		prevChanged = currChanged;
	}
}

void DynProg::addCharALeft(char c) {
	std::string aNew = "";
	aNew += c;
	a = aNew + a;
	dr.setMinRowIdx(dr.getMinRowIdx() - 1);
	updateDrRowwise();
}

void DynProg::addCharBLeft(char c) {
	std::string bNew = "";
	bNew += c;
	b = bNew + b;
	dr.setMinColIdx(dr.getMinColIdx() - 1);
	updateDrColwise();
}

/*void DynProg::removeCharARight() {
 a = a.substr(0, a.size() - 1);
 dr.setMaxRowIdx(dr.getMaxRowIdx() - 1);
 }

 void DynProg::removeCharBRight() {
 b = b.substr(0, b.size() - 1);
 dr.setMaxColIdx(dr.getMaxColIdx() - 1);
 }

 void DynProg::removeCharALeft() {
 a = a.substr(1, a.size() - 1);
 dr.setMinRowIdx(dr.getMinRowIdx() + 1);
 updateDrRowwise();
 }

 void DynProg::removeCharBLeft() {
 b = b.substr(1, b.size() - 1);
 dr.setMinColIdx(dr.getMinColIdx() + 1);
 updateDrColwise();
 }*/

void DynProg::printUMatrix() {
	std::cout << "U matrix begin:\n";
	for (int i = dr.getMinRowIdx() + 1; i <= dr.getMaxRowIdx(); ++i) {
		for (int j = dr.getMinColIdx() + 1; j <= dr.getMaxColIdx(); ++j) {
			std::cout << dr[i][j].u << " ";
		}
		std::cout << "\n";
	}
	std::cout << "U matrix end.\n";
}

void DynProg::printLMatrix() {
	std::cout << "L matrix begin:\n";
	for (int i = dr.getMinRowIdx() + 1; i <= dr.getMaxRowIdx(); ++i) {
		for (int j = dr.getMinColIdx() + 1; j <= dr.getMaxColIdx(); ++j) {
			std::cout << dr[i][j].l << " ";
		}
		std::cout << "\n";
	}
	std::cout << "L matrix end.\n";
}

double DynProg::normalizedEditDistance() {
	int dist = editDistance();
	int alpha = std::max(conf.insertion_penalty, conf.deletion_penalty);
	return (2.0 * dist) / (alpha * (a.size() + b.size()) + dist);
}
