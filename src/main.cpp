/*
 * main.cpp
 *
 *  Created on: May 15, 2018
 *      Author: Sarah Lutteropp
 */


#include <iostream>
#include <string>
#include <deque>

#include "dist_config.hpp"
#include "dyn_prog.hpp"

int main() {
	DistConfig conf;
	DynProg dp("a", "b", conf);
	return 0;
}
