#pragma once

#include "ofMain.h"
class Denoiser {
public:
	Denoiser();
	void setPrev(ofPixels prev);
	ofPixels temporalFilter(ofPixels frame);
private:
	ofPixels previousFrame;
};
