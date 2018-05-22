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
class DynamicMatrix {
public:
	DynamicMatrix() {
		nrows = 0;
		ncols = 0;
		minRowIdx = 0;
		minColIdx = 0;
		maxRowIdx = -1;
		maxColIdx = -1;
	}

	T& operator [](const std::pair<int,int>& idx) {
		int i = idx.first;
		int j = idx.second;
		return rows[i - minRowIdx][j - minColIdx];
	}
	int getNRows() {
		return nrows;
	}
	int getNCols() {
		return ncols;
	}
	void printMatrix() {
		for (int i = 0; i < nrows; ++i) {
			for (int j = 0; j < ncols; ++j) {
				std::cout << rows[i][j] << " ";
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
		while (val < minRowIdx) {
			std::deque<T> newRow(ncols);
			rows.push_front(newRow);
			nrows++;
			minRowIdx--;
		}
	}
	void setMinColIdx(int val) {
		while (val < minColIdx) {
			for (int i1 = 0; i1 < nrows; ++i1) {
				T newEntry;
				rows[i1].push_front(newEntry);
			}
			ncols++;
			minColIdx--;
		}
	}
	void setMaxRowIdx(int val) {
		while (val > maxRowIdx) {
			std::deque<T> newRow(ncols);
			rows.push_back(newRow);
			nrows++;
			maxRowIdx++;
		}
	}
	void setMaxColIdx(int val) {
		while (val > maxColIdx) {
			for (int i1 = 0; i1 < nrows; ++i1) {
				T newEntry;
				rows[i1].push_back(newEntry);
			}
			ncols++;
			maxColIdx++;
		}
	}
private:
	std::deque<std::deque<T> > rows;
	int nrows;
	int ncols;
	int minRowIdx;
	int minColIdx;
	int maxColIdx;
	int maxRowIdx;
};
