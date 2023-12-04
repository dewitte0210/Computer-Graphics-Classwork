#pragma once
#include "mathFunctions.h"

class Material;
class HitRecord {
public:
	glm::vec3 hitPoint;
	glm::vec3 normal;
	shared_ptr<Material> mat;
	float t;
	bool frontFace;

	void setFaceNormal(ray& r, glm::vec3& outwardNormal) {
		// Sets the hit record normal vector.
		// NOTE: outwardNormal is assumed to be normalized
		frontFace = glm::dot(r.getDirection(), outwardNormal) < 0;
		normal = frontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable {
public:
	virtual ~Hittable() = default;
	virtual bool hit(ray& r, Interval rayT, HitRecord& rec) const = 0;
};
