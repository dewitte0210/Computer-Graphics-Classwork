#include "hittableList.h"

HittableList::HittableList() {};
HittableList::HittableList(shared_ptr<Hittable> object) { add(object); }
void HittableList::clear() { objects.clear(); }
void HittableList::add(shared_ptr<Hittable> object) { objects.push_back(object); }

bool HittableList::hit(ray& r, Interval rayT, HitRecord& rec) const {
	HitRecord tempRec;
	bool hitAnything = false;
	float closestSoFar = rayT.max;

	for (const auto& object : objects) {
		if (object->hit(r, Interval(rayT.min, closestSoFar), tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec;
		}
	}
	return hitAnything;
}
