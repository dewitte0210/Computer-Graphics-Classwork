#include "interval.h"

//Interval::Interval() : min(+infinity), max(-infinity) {}
Interval::Interval(float min, float max) : min(min), max(max) {}

bool Interval::conatins(float x) const {
	return min <= x && x <= max;
}

bool Interval::surrounds(float x) const {
	return min < x && x < max;
}

float Interval::clamp(float x) const {
	if (x < min) { return min; }
	if (x > max) { return max; }
	return x;
}
