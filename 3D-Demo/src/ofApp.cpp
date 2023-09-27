#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	torusMesh.load("Models/torus.ply");
	ofDisableArbTex();
	ofEnableDepthTest();
	torusShader.load("Shaders/torus.vert", "Shaders/torus.frag");
}

//--------------------------------------------------------------
void ofApp::update(){

}

void ofApp::reloadShaders() {
	torusShader.load("Shaders/torus.vert", "Shaders/torus.frag");
}
//--------------------------------------------------------------
void ofApp::draw(){
	using namespace glm;
	int width{ ofGetWindowWidth()};
	int height{ ofGetWindowHeight()};
	float aspectRatio = static_cast<float>(width) / height;

	mat4 projection{perspective(radians(90.0f), aspectRatio, 0.1f,10.0f)};
	mat4 view{};
	mat4 model{translate(vec3(0,0,-1)) * rotate(1.0f, vec3(1,1,1)) * scale(vec3(0.5f,0.5f,0.5f))};
	mat4 mvp{projection * view * model};
	torusShader.begin();
	torusShader.setUniformMatrix4f("mvp", mvp);
	torusMesh.draw();
	torusShader.end();
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
