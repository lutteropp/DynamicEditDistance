/*
 * dist_config.hpp
 *
 *  Created on: May 15, 2018
 *      Author: Sarah Lutteropp
 */

#pragma once

struct DistConfig {
	int substitution_penalty = 1;
	int insertion_penalty = 1;
	int deletion_penalty = 1;
};
