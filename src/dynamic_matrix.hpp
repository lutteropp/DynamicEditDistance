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

template<typename T>
class DynamicMatrix {
public:
	DynamicMatrix() {
		nrows = 0;
		internalRowOffset = 0;
		ncols = 0;
		internalColOffset = 0;
		externalRowOffset = 0;
		externalColOffset = 0;
	}
	T& operator [](std::array<int, 2> idx) {
		int i = idx[0] - externalRowOffset;
		int j = idx[1] - externalColOffset;

		while (i + internalRowOffset >= nrows) {
			std::deque<T> newRow(ncols);
			rows.push_back(newRow);
			nrows++;
		}
		while (i <= -1 * internalRowOffset) {
			std::deque<T> newRow(ncols);
			rows.push_front(newRow);
			nrows++;
		}
		while (j + internalColOffset >= ncols) { // TODO: THIS CHECK IS WRONG!!!
			for (int i1 = 0; i1 < nrows; ++i1) {
				T newEntry = 0;
				rows[i1].push_back(newEntry);
			}
			ncols++;
		}
		while (j <= -1 * internalColOffset) { // TODO: THIS CHECK IS WRONG!!!
			for (int i1 = 0; i1 < nrows; ++i1) {
				T newEntry = 0;
				rows[i1].push_front(newEntry);
			}
			ncols++;
		}

		if (i < 0) {
			internalRowOffset = std::max(internalRowOffset, -1 * i);
		}
		if (j < 0) {
			internalColOffset = std::max(internalColOffset, -1 * j);
		}

		return rows[i + internalRowOffset][j + internalColOffset];
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
	int internalRowOffset;
	int internalColOffset;
	int externalRowOffset;
	int externalColOffset;
};
