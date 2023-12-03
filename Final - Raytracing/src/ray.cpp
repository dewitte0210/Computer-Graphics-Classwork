#pragma once
#include "ray.h"

ray::ray() {}
ray::ray(const glm::vec3& origin, const glm::vec3& direction) : orig(origin), dir(direction) {}

glm::vec3 ray::getOrigin() { return orig; }
glm::vec3 ray::getDirection() { return dir; }
glm::vec3 ray::at(float t) { return orig + t * dir; }