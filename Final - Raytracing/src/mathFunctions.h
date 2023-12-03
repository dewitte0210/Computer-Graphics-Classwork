#pragma once
#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

inline float degreesToRadians(float degrees) {
	return degrees * pi / 180.0;
}

inline float randomFloat() {
	//returns a random real in [0,1).
	return rand() / (RAND_MAX + 1.0);
}

inline float randomFloat(float min, float max) {
	// Returns a random real in [min, max).
	return min + (max - min) * randomFloat();
}

#include "ray.h"
#include "interval.h"
