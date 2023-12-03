#pragma once
#include "ofMain.h"

// Class taken from "Raytracing in One Weekend" tutorial
class ray {
public:
	ray();
	ray(const glm::vec3& origin, const glm::vec3& direction);

	glm::vec3 getOrigin();
	glm::vec3 getDirection();
	glm::vec3 at(float t);

private:
	glm::vec3 orig;
	glm::vec3 dir;
};
