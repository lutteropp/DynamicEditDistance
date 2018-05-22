/*
 * DynamicMatrix.hpp
 *
 *  Created on: May 15, 2018
 *      Author: Sarah Lutteropp
 */

#pragma once

#include <deque>
#include <array>
#include <iostream>
#include <utility>

// this allows both positive and negative indices
// this matrix only grows, but never shrinks!

template<typename T>
class MatrixRow {
public:
	MatrixRow() {
		minColIdx = 0;
		maxColIdx = -1;
	}
	T& operator[](int idx) {
		if (idx >= 0) {
			return positiveColumns[idx];
		} else {
			return negativeColumns[-idx - 1];
		}
	}
	int getMinColIdx() {
		return minColIdx;
	}
	int getMaxColIdx() {
		return maxColIdx;
	}
	void setMinColIdx(int val) {
		negativeColumns.resize(-val);
		minColIdx = val;
	}
	void setMaxColIdx(int val) {
		positiveColumns.resize(val + 1);
		maxColIdx = val;
	}
private:
	std::vector<T> positiveColumns;
	std::vector<T> negativeColumns;
	int minColIdx;
	int maxColIdx;
};

template<typename T>
class DynamicMatrix {
public:
	DynamicMatrix() {
		minRowIdx = 0;
		minColIdx = 0;
		maxRowIdx = -1;
		maxColIdx = -1;
	}

	MatrixRow<T>& operator [](int idx) {
		if (idx >= 0) {
			return positiveRows[idx];
		} else {
			return negativeRows[-idx - 1];
		}
	}

	int getNRows() {
		return (maxRowIdx + 1) + (-minRowIdx);
	}
	int getNCols() {
		return (maxColIdx + 1) + (-minColIdx);
	}
	void printMatrix() {
		for (size_t i = 0; i < negativeRows.size(); ++i) {
			for (int j = minColIdx; j <= maxColIdx; ++j) {
				std::cout << negativeRows[i][j] << " ";
			}
			std::cout << "\n";
		}
		for (size_t i = 0; i < positiveRows.size(); ++i) {
			for (int j = minColIdx; j <= maxColIdx; ++j) {
				std::cout << positiveRows[i][j] << " ";
			}
			std::cout << "\n";
		}
	}
	int getMinRowIdx() {
		return minRowIdx;
	}
	int getMinColIdx() {
		return minColIdx;
	}
	int getMaxRowIdx() {
		return maxRowIdx;
	}
	int getMaxColIdx() {
		return maxColIdx;
	}

	void setMinRowIdx(int val) {
		negativeRows.resize(-val);
		for (size_t i = 0; i < negativeRows.size(); ++i) {
			negativeRows[i].setMinColIdx(minColIdx);
			negativeRows[i].setMaxColIdx(maxColIdx);
		}
		minRowIdx = val;
	}
	void setMaxRowIdx(int val) {
		positiveRows.resize(val + 1);
		for (size_t i = 0; i < positiveRows.size(); ++i) {
			positiveRows[i].setMinColIdx(minColIdx);
			positiveRows[i].setMaxColIdx(maxColIdx);
		}
		maxRowIdx = val;
	}

	void setMinColIdx(int val) {
		for (size_t i = 0; i < positiveRows.size(); ++i) {
			positiveRows[i].setMinColIdx(val);
		}
		for (size_t i = 0; i < negativeRows.size(); ++i) {
			negativeRows[i].setMinColIdx(val);
		}
		minColIdx = val;
	}

	void setMaxColIdx(int val) {
		for (size_t i = 0; i < positiveRows.size(); ++i) {
			positiveRows[i].setMaxColIdx(val);
		}
		for (size_t i = 0; i < negativeRows.size(); ++i) {
			negativeRows[i].setMaxColIdx(val);
		}
		maxColIdx = val;
	}

private:
	std::vector<MatrixRow<T> > positiveRows;
	std::vector<MatrixRow<T> > negativeRows;

	int minRowIdx;
	int minColIdx;
	int maxColIdx;
	int maxRowIdx;
};
