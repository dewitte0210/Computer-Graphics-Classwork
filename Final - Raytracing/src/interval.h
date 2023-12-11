#pragma once

class Interval {
public:
	double min, max;
	//Interval();
	Interval(float min, float max);
	bool conatins(float x) const;
	bool surrounds(float x) const;
	float clamp(float x) const;
//	static const Interval empty, universe;
};

//const static Interval empty(+infinity, -infinity);
//const static Interval universe(-infinity, +infinity);
