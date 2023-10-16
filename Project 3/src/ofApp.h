#pragma once

#include "ofMain.h"
struct CameraData {
	glm::vec3 pos;
	float rot;
	float fov;
};
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
private:
	ofShader terrainShader;

	ofVboMesh terrain;
	
	ofShortImage heightmap;

	CameraData cam;
	glm::vec3 velocity;
	glm::vec3 velocityWorldSpace;
	int speed{ 10 };

	int mouseX{ 0 };
	int mouseY{ 0 };
	float cameraHead{ 0 };
	float cameraTilt{ 0 };
	float sensitivity{ 0.6 };
	void updateCameraRotation(float dx, float dy);
};
