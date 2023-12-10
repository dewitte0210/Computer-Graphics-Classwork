#include "denoiser.h" 
Denoiser::Denoiser(){}

ofPixels Denoiser::denoise(ofPixels frame, ofPixels normals, ofPixels positions) {
	frame = aTrous(frame, 1);
	frame = aTrous(frame, 2);
	frame = aTrous(frame, 3);
	return frame;
}
ofPixels Denoiser::aTrous(ofPixels frame, int steps) {
	float width = frame.getWidth();
	float height = frame.getHeight();
	int kernel[5][5]{ {1,4,7,4,1},{4,16,26,16,4},{7,26,41,26,7},{4,16,26,16,4},{1,4,7,4,1} };
	Interval xInterval{ 0, width-1 };
	Interval yInterval{ 0,height-1 };
	ofPixels newFrame{};

	newFrame.allocate(width, height, OF_IMAGE_COLOR);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			glm::vec3 sum{ 0,0,0 };
			for (int i = -2; i <= 2; i++) {
				for (int j = -2; j <= 2; j++) {
					ofColor currentPixelColor{ frame.getColor(frame.getPixelIndex(xInterval.clamp(x+(j*steps)),yInterval.clamp(y+(i*steps))))};
					glm::vec3 currPixel{ currentPixelColor.r, currentPixelColor.g, currentPixelColor.b };
					sum.r += currPixel.r * kernel[i + 2][j + 2];
					sum.g += currPixel.g * kernel[i + 2][j + 2];
					sum.b += currPixel.b * kernel[i + 2][j + 2];
				}
			}
			sum = glm::vec3{ sum.r / 273, sum.g / 273, sum.b / 273};
			newFrame.setColor(newFrame.getPixelIndex(x, y), ofColor{sum.r,sum.g,sum.b});
		}
	}
	return newFrame;
}


