#include "sphere.h"

Sphere::Sphere(glm::vec3 center, float radius) : center(center), radius(radius) {}

bool Sphere::hit(ray& r, Interval rayT, HitRecord& rec)const {
	using namespace glm;
	vec3 oc = r.getOrigin() - center;
	float a = dot(r.getDirection(), r.getDirection());
	float bHalf = dot(oc, r.getDirection());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = bHalf * bHalf - a * c;
	if (discriminant < 0) {
		return false;
	}
	float sqrtd = sqrt(discriminant);

	// Find the neareset root that lies in the acceptable range
	float root = (-bHalf - sqrtd) / a;
	if (!rayT.surrounds(root)) {
		root = (-bHalf + sqrtd) / a;
		if (!rayT.surrounds(root)) { return false; }
	}

	rec.t = root;
	rec.hitPoint = r.at(rec.t); // point that we hit
	vec3 outwardNormal = (rec.hitPoint - center) / radius;
	rec.setFaceNormal(r, outwardNormal);
	return true;
}