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
		externalRowOffset = 0;
		externalColOffset = 0;
	}
	T& operator [](std::array<int, 2> idx) {
		int i = idx[0] - externalRowOffset;
		int j = idx[1] - externalColOffset;

		while (i > maxRowIdx) {
			std::deque<T> newRow(ncols);
			rows.push_back(newRow);
			nrows++;
			maxRowIdx++;
		}
		while (i < minRowIdx) {
			std::deque<T> newRow(ncols);
			rows.push_front(newRow);
			nrows++;
			minRowIdx--;
		}
		while (j > maxColIdx) {
			for (int i1 = 0; i1 < nrows; ++i1) {
				T newEntry;
				rows[i1].push_back(newEntry);
			}
			ncols++;
			maxColIdx++;
		}
		while (j < minColIdx) {
			for (int i1 = 0; i1 < nrows; ++i1) {
				T newEntry;
				rows[i1].push_front(newEntry);
			}
			ncols++;
			minColIdx--;
		}

		return rows[i - minRowIdx][j - minColIdx];
	}
	int getNRows() {
		return nrows;
	}
	int getNCols() {
		return ncols;
	}
	void setExternalRowOffset(int offset) {
		externalRowOffset = offset;
	}
	void setExternalColOffset(int offset) {
		externalColOffset = offset;
	}
	void printMatrix() {
		for (int i = 0; i < nrows; ++i) {
			for (int j = 0; j < ncols; ++j) {
				std::cout << rows[i][j] << " ";
			}
			std::cout << "\n";
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
	int externalRowOffset;
	int externalColOffset;
};
