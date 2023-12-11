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
inline glm::vec3 reflect(const glm::vec3& ray, const glm::vec3& normal) {
	return ray - 2 * glm::dot(ray, normal) * normal;
}
inline glm::vec3 refract(const glm::vec3& uv, const glm::vec3& n, float etaiOverEtat) {
	float cosTheta{ fmin(glm::dot(-uv,n), 1.0f) };
	glm::vec3 perpendicularR = etaiOverEtat * (uv + cosTheta * n);
	glm::vec3 parallelR = -sqrt(fabs(1.0 - glm::dot(perpendicularR, perpendicularR))) * n;
	return perpendicularR + parallelR;
}

#include "ray.h"
#include "interval.h"
ofColor getFinalColor(glm::vec3 pixel,int samples) {
	float r = pixel.r;
	float g = pixel.g;
	float b = pixel.b;
	float scale = 1.0 / samples;
	r *= scale; g *= scale; b *= scale;
	
	//Gamma correct the color.
	r = glm::pow(r, 1.0 / 2.2);
	g = glm::pow(g, 1.0 / 2.2);
	b = glm::pow(b, 1.0 / 2.2);

	ofColor finalColor;
	Interval intensity{ 0.000, 0.999 };
	finalColor.r = static_cast<int>(256 * intensity.clamp(r));
	finalColor.g = static_cast<int>(256 * intensity.clamp(g));
	finalColor.b = static_cast<int>(256 * intensity.clamp(b));
	return finalColor;
}

