#include "denoiser.h" 
Denoiser::Denoiser(){}

ofPixels Denoiser::denoise(ofPixels& frame, ofPixels& normals, ofPixels& positions) {
	width = frame.getWidth();
	height = frame.getHeight();
	ofPixels frameBuffer = frame;
	for (int i = 1; i <= 1; i++) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				ofColor temp = aTrous(frameBuffer, normals, positions, i, x, y);
				frameBuffer.setColor(frameBuffer.getPixelIndex(x, y), temp);
			}
		}
	}
	return frameBuffer;
}

ofColor Denoiser::aTrous(ofPixels frame, ofPixels& normals, ofPixels& positions, int steps, int x, int y) {
	int kernel[5][5]{ {1,4,7,4,1},{4,16,26,16,4},{7,26,41,26,7},{4,16,26,16,4},{1,4,7,4,1} };
	Interval xInterval{ 0, static_cast<float>(width) };
	Interval yInterval{ 0,static_cast<float>(height) };
	auto index = frame.getPixelIndex(x, y);
	ofColor targetPixel{ frame.getColor(index) };
	ofColor targetNormal{ normals.getColor(index) };
	ofColor targetPosition{ positions.getColor(index) };

	glm::vec3 sum{ 0,0,0 };
	float totalWeight{ 0 };
	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			auto kIndex = frame.getPixelIndex(xInterval.clamp(x + (j * glm::pow(2, steps-1))), yInterval.clamp(y + (i * glm::pow(2, steps-1))));
			ofColor currPixel{ frame.getColor(kIndex) };
			ofColor currNormal{ normals.getColor(kIndex) };
			ofColor currPosition{ positions.getColor(kIndex) };

			ofColor pixel{ targetPixel - currPixel };
			glm::vec3 diff{ pixel.r,pixel.g,pixel.b };
			float dist = glm::dot(diff, diff);
			float SIG = rtSIG * glm::pow(2, -steps);
			float weightRT = glm::min(glm::exp(-dist / SIG), 1.0f);

			ofColor normal{ targetNormal - currNormal };
			diff = { normal.r,normal.g,normal.b };
			dist = glm::max(glm::dot(diff, diff) / (steps*steps), 0.0f);
			float weightN = glm::min(glm::exp(-dist / nSIG), 1.0f);
			
			ofColor pos{ targetPosition - currPosition };
			diff = {pos.r, pos.g, pos.b};
			dist = glm::dot(diff, diff);
			float weightP = glm::min(glm::exp(-dist/ pSIG), 1.0f);
                                                                                                                                                        
			float pixelWeight = weightRT * weightN * weightP;

			sum.r += currPixel.r * kernel[i + 2][j + 2] * pixelWeight;
			sum.g += currPixel.g * kernel[i + 2][j + 2] * pixelWeight;
			sum.b += currPixel.b * kernel[i + 2][j + 2] * pixelWeight;
			totalWeight += kernel[i + 2][j + 2] * pixelWeight;
		}
	}
	sum = sum / 273;
	ofColor final{ getFinalColor(sum / totalWeight,1) };
	return final;
}


