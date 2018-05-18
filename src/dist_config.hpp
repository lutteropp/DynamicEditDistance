/*
 * dist_config.hpp
 *
 *  Created on: May 15, 2018
 *      Author: Sarah Lutteropp
 */

#pragma once

class DistConfig {
public:
	DistConfig(int sub, int ins, int del) {
		substitution_penalty = sub;
		insertion_penalty = ins;
		deletion_penalty = del;
	}
	DistConfig() {
		substitution_penalty = 1;
		insertion_penalty = 1;
		deletion_penalty = 1;
	}

	int substitution_penalty;
	int insertion_penalty;
	int deletion_penalty;
};
