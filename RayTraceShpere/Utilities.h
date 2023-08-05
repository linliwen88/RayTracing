#pragma once
#include <cstdlib>

inline double random_double() {
	// return a random real in [0, 1)
	return rand() / (RAND_MAX + 1.0);
}
