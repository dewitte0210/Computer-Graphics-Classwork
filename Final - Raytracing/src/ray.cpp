#pragma once
#include "ray.h"

ray::ray() {}
ray::ray(const glm::vec3& origin, const glm::vec3& direction) : orig(origin), dir(direction) {}

glm::vec3 ray::getOrigin() const { return orig; }
glm::vec3 ray::getDirection() const { return dir; }
glm::vec3 ray::at(float t) const { return orig + t * dir; }