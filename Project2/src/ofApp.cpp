#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();
	staff.load("assets/staff.ply");
	meshShader.load("shaders/mesh.vert", "shaders/mesh.frag");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	using namespace glm;

	cam.pos = vec3(0, 0, 1);
	cam.fov = radians(100.0f);
	float aspect = static_cast<float>(ofGetWindowWidth()) / ofGetWindowHeight();

	mat4 model = mat4() * scale(vec3(0.25,0.25,0.25));
	mat4 view = inverse(translate(cam.pos));
	mat4 proj = perspective(cam.fov, aspect, 0.01f, 10.0f);
	mat4 mvp = proj * view * model;

	meshShader.begin();
	meshShader.setUniformMatrix4f("mvp", mvp);
	staff.draw();
	meshShader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_UP)
	{
		
	}
	else if (key == OF_KEY_DOWN)
	{

	}
	else if (key == OF_KEY_RIGHT)
	{

	}
	else if (key == OF_KEY_LEFT)
	{

	}
	else if (key == OF_KEY_RIGHT_CONTROL)
	{

	}
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
