#pragma once
#include "ofMain.h"
#include "mathFunctions.h"

class HitRecord {
public:
	glm::vec3 hitPoint;
	glm::vec3 normal;
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
