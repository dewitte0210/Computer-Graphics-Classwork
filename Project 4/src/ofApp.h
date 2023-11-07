#pragma once

#include "ofMain.h"
#include "Camera.h"
#include "SceneGraphNode.h"
#include "SpotLightNode.h"
#include "Lighting.h"
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
		void updateCameraRotation(float dx, float dy);
	private:
		Camera camera;
		glm::vec3 velocity{ 0,0,0 };
		glm::vec3 velocityWorldSpace;
		int speed{ 5 };
		float sensitivity{ 0.6f };
		int mouseX{ 0 };
		int mouseY{ 0 };
		bool reload{ false };
		ofShader robotShader;

		Lighting lighting;
		ofMesh arm;
		ofMesh body;
		ofMesh head;
		ofMesh eye;
		ofMesh wheel;
		
		// root node of the scene graph
		SceneGraphNode sceneGraphRoot{};
		std::shared_ptr<SpotLightNode> spotLightNode{ new SpotLightNode{} };
};
