#include "ofApp.h"
#include "mathFunctions.h"
#include "hittable.h"
#include "materials.h"
#include "hittableList.h"
#include "sphere.h"
#include <thread>
#include <future>
#include <vector>


/* Paper for Denoising
https://vciba.springeropen.com/articles/10.1186/s42492-019-0016-7 */
glm::vec3 rayColor(ray& r, const Hittable& world, int depth) {
	if (depth <= 0) { return glm::vec3(0, 0, 0); }
	glm::vec3 color{ 0,0,0 };
	HitRecord rec;

	if (world.hit(r,Interval(0.001, infinity), rec)) {
		ray scattered;
		glm::vec3 attenuation;
		if (rec.mat->scatter(r, rec, attenuation, scattered)) {
			return attenuation * rayColor(scattered, world, depth - 1);
		}
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
	imageHeight = static_cast<int>(imageWidth / aspectRatio);
	imageHeight = (imageHeight < 1) ? 1 : imageHeight;

	// Camera
	float focalLength = (lookFrom - lookAt).length();
	float theta = degreesToRadians(camFov);
	float h = tan(theta / 2);
	float viewportHeight = 2.0 * h * focalLength;
	float viewportWidth = viewportHeight * (static_cast<float>(imageWidth) / imageHeight);
	cameraCenter = lookFrom;

	//Calculate the u,v,w unit vecrots for the camera coordinate frame;
	w = normalize(lookFrom - lookAt);
	u = normalize(cross(vUp, w));
	v = cross(w, u);

	//Calculate the vectors acroos the horizontlal and down the vertical viewport edges.
	vec3 viewportU = viewportWidth * u;
	vec3 viewportV = viewportHeight * -v;

	//Calculate the horizontal and vertical delta vectors from pixel to pixel
	pixelDeltaU = viewportU / imageWidth;
	pixelDeltaV = viewportV / imageHeight;

	//Calculate the location of the upper left pixel.
	vec3 viewportUpperLeft = cameraCenter - (focalLength * w) - viewportU/2 - viewportV/2;
	pixel00Loc = viewportUpperLeft + 0.5 * (pixelDeltaU + pixelDeltaV);

	//Three materials Test Image
	auto groundMaterial = make_shared<Lambertian>(glm::vec3(0.8, 0.8, 0.0));
	auto centerMaterial = make_shared<Lambertian>(glm::vec3(0.7, 0.3, 0.3));
	auto leftMaterial = make_shared<Mirror>(glm::vec3(0.8, 0.8, 0.8), 0.0);
	auto rightMaterial = make_shared<Mirror>(glm::vec3(0.8, 0.6, 0.2),1.0);

	world.add(make_shared<Sphere>(glm::vec3(0.0, -100.5, -1.0), 100.0, groundMaterial));
	world.add(make_shared<Sphere>(glm::vec3(0.0, 0.0, -1.0), 0.5, centerMaterial));
	world.add(make_shared<Sphere>(glm::vec3(-1.0, 0.0, -1.0), 0.5, leftMaterial));
	world.add(make_shared<Sphere>(glm::vec3(1.0, 0.0, -1.0), 0.5, rightMaterial));
	
	
	/* Random Balls Image
	//Add objects to the world
	auto groundMaterial = make_shared<Lambertian>(glm::vec3(0.5, 0.5, 0.5));
	world.add(make_shared<Sphere>(glm::vec3(0, -1000, 0), 1000, groundMaterial));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float chooseMat = randomFloat();
			vec3 center(a + 0.9 * randomFloat(), 0.2, b + 0.9 * randomFloat());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				shared_ptr<Material> sphereMat;
				if (chooseMat < 0.8) {
					//Diffuse Material
					vec3 albedo{ randomFloat(),randomFloat(),randomFloat() };
					sphereMat = make_shared<Lambertian>(albedo);
					world.add(make_shared<Sphere>(center, 0.2, sphereMat));
				}
				else if (chooseMat < 0.95) {
					//Mirror Material
					vec3 albedo{ randomFloat(), randomFloat(), randomFloat() };
					float fuzz{ randomFloat(0,0.5) };
					sphereMat = make_shared<Mirror>(albedo, fuzz);
				}
				else {
					//Glass ball
					sphereMat = make_shared<Dielectric>(1.5);
				}
				world.add(make_shared<Sphere>(center, 0.2, sphereMat));
			}
		}
	}
	auto dielectricMat = make_shared<Dielectric>(1.5);
	world.add(make_shared<Sphere>(vec3(0, 1, 0), 1.0, dielectricMat));
	
	auto lambertianMat = make_shared<Lambertian>(vec3(0.5, 0.2, 0.1));
	world.add(make_shared<Sphere>(vec3(-4, 1, 0), 1.0, lambertianMat));

	auto mirrorMat = make_shared<Mirror>(vec3(0.7, 0.5, 0.5), 0.0);
	world.add(make_shared<Sphere>(vec3(4, 1, 0), 1.0, mirrorMat));
	*/
	frameBuffer.allocate(imageWidth, imageHeight, OF_IMAGE_COLOR);
}

//--------------------------------------------------------------
void ofApp::update(){

}

void threadedRayColor(ray& r,const Hittable& world, int depth, std::promise<glm::vec3> & prms) {
	prms.set_value(rayColor(r, world, depth));
}

void ofApp::calcPixel(int x, int y, int samples, std::promise<glm::vec3> & prms) {
	glm::vec3 pixelColor{ 0,0,0 };
	for (int i = 0; i < samples; i++) {
		ray r = getRay(x, y);
		pixelColor += rayColor(r, world, maxDepth);
	}
	prms.set_value(pixelColor);
}
//--------------------------------------------------------------
void ofApp::draw(){
	using namespace glm;
// Original single thread implementation
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
// Multi threaded implementation
	/*
	for (int y = 0; y < imageHeight; y++) {
		std::clog << "\r Scanlines remaining: " << (imageHeight - y) << ' ' << std::flush;
		std::vector<std::thread> threads;
		std::vector<std::future<glm::vec3>> futures;
		for (int x = 0; x < imageWidth; x++) {
			std::promise<glm::vec3> prms;
			std::future<glm::vec3> ftr = prms.get_future();
			threads.push_back(std::thread(&ofApp::calcPixel, x, y, samplesPerPixel, std::move(prms)));
			futures.push_back(std::move(ftr));
		}
		int count = 0;
		for (auto& th : threads) {
			frameBuffer.setColor(frameBuffer.getPixelIndex(count, y), getFinalColor(futures.at(count).get(), samplesPerPixel));
			th.join();
		}
	}
	*/
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
