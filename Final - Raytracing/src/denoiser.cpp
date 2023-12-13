#include "denoiser.h" 
Denoiser::Denoiser(){}

ofPixels Denoiser::denoise(ofPixels& frame, ofPixels& normals, ofPixels& positions) {
	width = frame.getWidth();
	height = frame.getHeight();
	ofPixels frameBuffer = frame;
	for (int i = 1; i <= 3; i++) {
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
	using namespace glm;

//	int kernel[5][5]{ {1,4,7,4,1},{4,16,26,16,4},{7,26,41,26,7},{4,16,26,16,4},{1,4,7,4,1} };
	int kernel[3][3]{ {1,2,1}, {2,4,2}, {1,2,1} };
	Interval xInterval{ 0, static_cast<float>(width) };
	Interval yInterval{ 0,static_cast<float>(height) };
	auto index = frame.getPixelIndex(x, y);
	ofColor targetPixel{ frame.getColor(index) };
	ofColor targetNormal{ normals.getColor(index) };
	ofColor targetPosition{ positions.getColor(index) };
	float stepWidth = glm::pow(2, steps - 1);
	vec3 sum{ 0,0,0 };
	float totalWeight{ 0 };
	//Code for gaussian blur
//	for (int i = -2; i <= 2; i++) {
	for(int i = -1; i <= 1; i++){
	//	for (int j = -2; j <= 2; j++) {
		for(int j = -1; j <= 1; j++){
			index = frame.getPixelIndex(xInterval.clamp(x + (j * stepWidth)), yInterval.clamp(y + (i * stepWidth)));
			ofColor currPixel{ frame.getColor(index) };
			ofColor currNormal{ normals.getColor(index) };
			ofColor currPosition{ positions.getColor(index) };

			ofColor pixel{ targetPixel - currPixel };
			glm::vec3 diff{ pixel.r,pixel.g,pixel.b };
			float dist = dot(diff, diff);
			float SIG = rtSIG * pow(2, -steps);
			float weightRT = glm::min(exp(-dist / pow(SIG,2.0f)), 1.0f);
			
			
			ofColor normal{ targetNormal - currNormal };
			diff = { normal.r,normal.g,normal.b };
			dist = glm::max(dot(diff, diff) / (stepWidth * stepWidth), 0.0f);
			float weightN = glm::min(exp(-dist /pow(nSIG,2.0f)), 1.0f);
		
		/*
			ofColor pos{ targetPosition - currPosition };
			diff = {pos.r, pos.g, pos.b};
			dist = glm::dot(diff, diff);
			float weightP = glm::min(exp(-dist/ pow(pSIG, 2.0f)), 1.0f);
			*/

			float pixelWeight = weightRT * weightN;// *weightP;

			sum.r += currPixel.r * kernel[i + 1][j + 1] * pixelWeight;
			sum.g += currPixel.g * kernel[i + 1][j + 1] * pixelWeight;
			sum.b += currPixel.b * kernel[i + 1][j + 1] * pixelWeight;
			totalWeight += kernel[i + 1][j + 1] * pixelWeight;
		}
	}
	sum = sum / totalWeight;
	ofColor final{ sum.r, sum.g, sum.b };
	return final;
}

/*code for calculating positional weights

*/


