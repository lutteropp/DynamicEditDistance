/*
 * DynamicMatrix.hpp
 *
 *  Created on: May 15, 2018
 *      Author: Sarah Lutteropp
 */

#pragma once

#include <deque>
#include <array>

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
		if (i < 0) {
			internalRowOffset = std::min(internalRowOffset, -1*i);
		}
		if (j < 0) {
			internalColOffset = std::min(internalColOffset, -1*i);
		}

		while (i + internalRowOffset >= nrows) {
			std::deque<T> newRow(ncols);
			rows.push_back(newRow);
			nrows++;
		}
		if (j + internalColOffset >= ncols) {
			for (int i1 = 0; i1 < nrows; ++i1) {
				rows[i1].resize(j + internalColOffset + 1);
			}
			ncols = j + internalColOffset;
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
private:
	std::deque<std::deque<T> > rows;
	int nrows;
	int ncols;
	int internalRowOffset;
	int internalColOffset;
	int externalRowOffset;
	int externalColOffset;
};
