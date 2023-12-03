#pragma once
#include "hittable.h"
#include "ofMain.h"

class Sphere : public Hittable {
public :
	Sphere(glm::vec3 center, float radius, shared_ptr<Material> material);
	bool hit(ray& r, Interval rayT, HitRecord& rec) const;
private:
	glm::vec3 center;
	float radius;
	shared_ptr<Material> mat;
};
