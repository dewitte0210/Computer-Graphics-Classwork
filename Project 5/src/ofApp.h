#pragma once

#include "ofMain.h"
#include "CellManager.h"
#include "ofxCubemap.h"
#include "Camera.h"
#include "CameraMatrices.h"

struct DirectionalLight{
	glm::vec3 direction;
	glm::vec3 lightColor;
};
struct PointLight {
	glm::vec3 position;
	glm::vec3 lightColor;
};
class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();
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
		void reloadShaders();
		void drawCube(const CameraMatrices& camera);
private:
	ofShader terrainShader;
	ofShader waterShader;
	ofShader skyboxShader;
	
	ofVboMesh terrain;
	ofVboMesh water;
	ofMesh cubeMesh;

	ofShortImage heightmap;
	ofShortImage highResHeightmap;
	ofImage terrainTex;
	ofImage terrainNrml;
	ofImage waterNrml;
	ofxCubemap cubeMap;
	ofxCubemap irradianceMap;

	bool needsReload{ false };

	const glm::vec3 TERRAIN_COLOR{ 0.2, 1,0.4 };
	const glm::vec3 WATER_COLOR{ 0.1,0.1,0.1 };
	const glm::vec3 AMBIENT_LIGHT{ 0.3 };
	const int WATER_HEIGHT{ 700 };
	const glm::vec3 SPECULAR_COLOR{ 0.02 };
	CellManager<5> cellManager{ highResHeightmap, 1600, 256 };
	DirectionalLight mainLight;
	PointLight pointLight;

	Camera camera;
	glm::vec3 velocity;
	glm::vec3 velocityWorldSpace;
	int speed{ 150 };
	int mouseX{ 0 };
	int mouseY{ 0 };
	float cameraHead{ 0 };
	float cameraTilt{ 0 };
	float sensitivity{ 0.6 };
	float t{ 0.0f };
	void updateCameraRotation(float dx, float dy);
};
