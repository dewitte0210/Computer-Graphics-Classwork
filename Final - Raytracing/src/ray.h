#pragma once
#include "ofMain.h"

// Class taken from "Raytracing in One Weekend" tutorial
class ray {
public:
	ray();
	ray(const glm::vec3& origin, const glm::vec3& direction);

	glm::vec3 getOrigin() const;
	glm::vec3 getDirection() const;
	glm::vec3 at(float t) const;

private:
	glm::vec3 orig;
	glm::vec3 dir;
};
