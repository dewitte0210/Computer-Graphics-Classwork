#pragma once

#include "ofMain.h"
#include "interval.h"
class Denoiser {
public:
	Denoiser();
	void setPrev(ofPixels prev);
	ofPixels denoise(ofPixels frame, ofPixels normals, ofPixels positions);
	ofPixels aTrous(ofPixels frame, int kernelSize);
private:
	int calcBox(int kernel[3][3], int pixels[3][3]);
	ofPixels previousFrame;

};
