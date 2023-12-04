#include "ofApp.h"
#include "mathFunctions.h"
#include "hittable.h"
#include "materials.h"
#include "hittableList.h"
#include "sphere.h"

glm::vec3 rayColor(ray& r, const Hittable& world, int depth) {
	if (depth <= 0) { return glm::vec3(0, 0, 0); }
	glm::vec3 color;
	HitRecord rec;

	if (world.hit(r,Interval(0.001, infinity), rec)) {
		ray scattered;
		glm::vec3 attenuation;
		if (rec.mat->scatter(r, rec, attenuation, scattered)) {
			return attenuation & rayColor(scattered, world, depth - 1);
		}
		return glm::vec3(0, 0, 0);
	} else {
		glm::vec3 unitDirection{ glm::normalize(r.getDirection()) };
		float a = 0.5 * (unitDirection.y + 1.0);
		color = glm::vec3((1.0 - a) * glm::vec3(1.0,1.0,1.0) + a * glm::vec3(0.5,0.7,1.0));
	}

	return color;
}
ray ofApp::getRay(int x, int y) {
	glm::vec3 pixelCenter = pixel00Loc + (x * pixelDeltaU) + (y * pixelDeltaV);
	glm::vec3 squareSample = ((-0.5 + randomFloat()) * pixelDeltaU) + ((-0.5 + randomFloat()) * pixelDeltaV);
	glm::vec3 pixelSample = pixelCenter + squareSample;
	glm::vec3 rayDirection = pixelSample - cameraCenter;
	return ray(cameraCenter, rayDirection);
}

ofColor ofApp::getFinalColor(glm::vec3 pixel,int samples) {
	float r = pixel.r;
	float g = pixel.g;
	float b = pixel.b;
	float scale = 1.0 / samples;
	r *= scale; g *= scale; b *= scale;
	
	//Gamma correct the color.
	r = glm::pow(r, 1.0 / 2.2);
	g = glm::pow(g, 1.0 / 2.2);
	b = glm::pow(b, 1.0 / 2.2);

	ofColor finalColor;
	Interval intensity{ 0.000, 0.999 };
	finalColor.r = static_cast<int>(256 * intensity.clamp(r));
	finalColor.g = static_cast<int>(256 * intensity.clamp(g));
	finalColor.b = static_cast<int>(256 * intensity.clamp(b));
	return finalColor;
}
//--------------------------------------------------------------
void ofApp::setup(){
	using namespace glm;
	// setup the frameBuffer and viewport settings
	aspectRatio = 16.0 / 9.0;
	imageHeight = static_cast<int>(imageWidth / aspectRatio);
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;

	// Camera
	float viewportHeight = 2.0;
	float viewportWidth = viewportHeight * (static_cast<float>(imageWidth) / imageHeight);
	float focalLength = 1.0;
	cameraCenter = glm::vec3(0, 0, 0);
	
	//Calculate the vectors acroos the horizontlal and down the vertical viewport edges.
	vec3 viewportU = vec3(viewportWidth, 0, 0);
	vec3 viewportV = vec3(0, -viewportHeight, 0);

	//Calculate the horizontal and vertical delta vectors from pixel to pixel
	pixelDeltaU = viewportU / imageWidth;
	pixelDeltaV = viewportV / imageHeight;

	//Calculate the location of the upper left pixel.
	vec3 viewportUpperLeft = cameraCenter - vec3(0, 0, focalLength) - viewportU / 2 - viewportV / 2;
	pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

	//Add objects to the world
	auto materialGround{ make_shared<Lambertian>(glm::vec3(0.8,0.8,0.0)) };
	auto materialCenter{ make_shared<Lambertian>(glm::vec3(0.7,0.3,0.3)) };
	auto materialLeft{ make_shared<Mirror>(glm::vec3(0.8,0.8,0.8)) };
	auto materialRight{ make_shared<Mirror>(glm::vec3(0.8,0.6,0.2)) };

	world.add(make_shared<Sphere>(glm::vec3(0.0, -100.5, -1.0), 100.0, materialGround));
	world.add(make_shared<Sphere>(glm::vec3(0.0, 0.0, -1.0), 0.5, materialCenter));
	world.add(make_shared<Sphere>(glm::vec3(-1.0, 0.0, -1.0), 0.5, materialLeft));
	world.add(make_shared<Sphere>(glm::vec3(1.0, 0.0, -1.0), 0.5, materialRight));
	frameBuffer.allocate(imageWidth, imageHeight, OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	using namespace glm;
	
	for (int y = 0; y < imageHeight; y++) {
		std::clog << "\rScanlines remaining: " << (imageHeight- y) << ' ' << std::flush;
		for (int x = 0; x < imageWidth; x++) {
			glm::vec3 pixelColor{ 0,0,0 };
			for (int sample = 0; sample < samplesPerPixel; sample++) {
				ray r = getRay(x, y);
				pixelColor += rayColor(r, world, maxDepth);
			}
			frameBuffer.setColor(frameBuffer.getPixelIndex(x, y), getFinalColor(pixelColor, samplesPerPixel));
		}
	}
	std::clog << "\rDone.                 \n";
	display.setFromPixels(frameBuffer);
	display.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
