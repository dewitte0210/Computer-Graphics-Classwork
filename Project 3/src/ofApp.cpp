#include "ofApp.h"
#include "buildTerrainMesh.h"
void ofApp::updateCameraRotation(float dx, float dy) {
	cameraHead += dx * ofGetLastFrameTime();
	cameraTilt += dy * ofGetLastFrameTime();
}
//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();
	ofEnableDepthTest();
	glEnable(GL_CULL_FACE);
	cam.pos = glm::vec3(0, 0, 1);
	velocity = glm::vec3(0, 0, 0);
}

//--------------------------------------------------------------
void ofApp::update(){
	velocityWorldSpace = glm::mat3(glm::rotate(-cameraHead, glm::vec3(0, 1, 0))) * velocity * ofGetLastFrameTime();
}

//--------------------------------------------------------------
void ofApp::draw(){
	using namespace glm;
	std::cout << ofGetFrameRate() << std::endl;

	cam.fov = radians(100.0f);
	float aspect = static_cast<float>(ofGetWindowWidth()) / ofGetWindowHeight();
	cam.pos += velocityWorldSpace * speed;

	mat4 view{ rotate(cameraTilt, vec3(1,0,0)) * rotate(cameraHead, vec3(0,1,0)) * translate(-cam.pos) };
	mat4 projection{ perspective(radians(100.0f), aspect, 0.01f, 10.0f) };

	terrainShader.begin();
	terrainShader.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ')
	{
		velocity.y = 1;
	}
	else if (key == OF_KEY_LEFT_CONTROL)
	{
		velocity.y = -1;
	}
	else if (key == 'd')
	{
		velocity.x = 1;
	}
	else if (key == 'a')
	{
		velocity.x = -1;
	}
	else if (key == 'w')
	{
		velocity.z = -1;
	}
	else if (key == 's')
	{
		velocity.z = 1;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == OF_KEY_LEFT_CONTROL || key == ' ')
	{
		velocity.y = 0;
	}
	else if (key == 'd' || key == 'a')
	{
		velocity.x = 0;
	}
	else if (key == 'w' || key == 's')
	{
		velocity.z = 0;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	if (mouseX != 0 && mouseY != 0) {
		updateCameraRotation(sensitivity * (x - mouseX), sensitivity * (y - mouseY));
	}
	mouseX = x;
	mouseY = y;
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
