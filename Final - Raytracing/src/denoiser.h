#pragma once

#include "mathFunctions.h"
#include "interval.h"
class Denoiser {
public:
	Denoiser();
	ofPixels denoise(ofPixels& frame, ofPixels& normals, ofPixels& positions);
	ofColor aTrous(ofPixels frame, ofPixels& normals, ofPixels& positions, int steps, int x, int y);
private:
	int width;
	int height;
	float rtSIG{ 0.850};
	float nSIG{ 0.100};
	float pSIG{ 0.000001};

};
