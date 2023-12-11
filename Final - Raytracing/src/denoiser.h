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
	float rtSIG{ 0.950};
	float nSIG{ 0.300};
	float pSIG{ 0.250};

};
