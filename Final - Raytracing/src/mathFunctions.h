#pragma once
#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include "ofMain.h"

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

//Functions that should be added to the vector class but are here since im using glm
glm::vec3 randomVec() {
	return glm::vec3(randomFloat(), randomFloat(), randomFloat());
}

glm::vec3 randomVec(float min, float max) {
	return glm::vec3(randomFloat(min, max), randomFloat(min, max), randomFloat(min, max));
}

glm::vec3 reflect(const glm::vec3& ray, const glm::vec3& normal) {
	return ray - 2 * glm::dot(ray, normal) * normal;
}

bool nearZero(glm::vec3 vec) {
	auto s = 1e-8;
	return (fabs(vec.x) < s) && (fabs(vec.y) < s) && (fabs(vec.z) < s);
}
inline glm::vec3 randomInUnitSphere() {
	while (true) {
		glm::vec3 p = randomVec(-1, 1);
		if (glm::dot(p, p) < 1) {
			return p;
		}
	}
}
inline glm::vec3 randomUnitVector() {
	return glm::normalize(randomInUnitSphere());
}
inline glm::vec3 randomOnHemisphere(const glm::vec3& normal) {
	glm::vec3 onUnitSphere = randomUnitVector();
	if (glm::dot(onUnitSphere, normal) > 0.0) {
		return onUnitSphere;
	}
	else {
		return -onUnitSphere;
	}
}

#include "ray.h"
#include "interval.h"
