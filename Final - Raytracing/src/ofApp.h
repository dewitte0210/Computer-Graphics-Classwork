#pragma once

#include "ofMain.h"
#include "hittablelist.h"
#include "mathFunctions.h"
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		ray getRay(int x, int y);
		ofColor getFinalColor(glm::vec3 pixel, int samples);
private:
	ofImage display;
	ofPixels frameBuffer;

	int imageHeight;
	int imageWidth{ 1200 };
	float aspectRatio{ 16.0 / 9.0 };
	int samplesPerPixel{ 10 };
	int maxDepth{ 50 }; //Maximum number of ray bounces into the scene

	//Camera Specific Variables
	glm::vec3 cameraCenter;
	glm::vec3 pixel00Loc;
	glm::vec3 pixelDeltaU;
	glm::vec3 pixelDeltaV;
	glm::vec3 lookFrom{ 13,2,3 };
	glm::vec3 lookAt{ 0,0,0 };
	glm::vec3 vUp{ 0,1,0 };
	glm::vec3 u, v, w;
	float camFov{ 20.0 };
	HittableList world;
};
