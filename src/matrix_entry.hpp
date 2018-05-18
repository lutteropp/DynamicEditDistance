/*
 * matrix_entry.hpp
 *
 *  Created on: May 15, 2018
 *      Author: Sarah Lutteropp
 */

#pragma once
#include <iostream>

class MatrixEntry {
public:
	int l;
	int u;
	friend std::ostream& operator<<(std::ostream& os, const MatrixEntry& dt) {
		os << "(" << dt.l << "," << dt.u << ")";
		return os;
	}
};
