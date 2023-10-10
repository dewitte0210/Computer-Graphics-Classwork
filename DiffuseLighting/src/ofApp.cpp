#include "ofApp.h"

float dot(glm::vec3 a, glm::vec3 b) {
	float x = a.x * b.x;
	float y = a.y * b.y;
	float z = a.z * b.z;
	return x + y + z;
}

float angleBetween(glm::vec3 a, glm::vec3 b) {
	float dotProduct = dot(a, b);
	float len = length(a) * length(b);
	float cosAngle = dotProduct / len;
	float angle = acos(cosAngle);
	return angle;
}

glm::vec3 getLightDirection(DirectionalLight& l) {
	return glm::normalize(l.direction * -1.0f);
}

glm::vec3 getLightColor(DirectionalLight& l){
	return l.color * l.intensity;
}
//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();
	ofEnableDepthTest();
	ofSetBackgroundColor(0, 0, 0);
	torusMesh.load("assets/torus.ply");
	uvShader.load("shaders/rimLight.vert", "shaders/rimLight.frag");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	using namespace glm;
	DirectionalLight dirLight;

	dirLight.direction = normalize(vec3(0, -1, 0));
	dirLight.color = vec3(1, 1, 1);
	dirLight.intensity = 1.0f;

	cam.pos = vec3(0.0, 0.75, 1.0);
	cam.fov = radians(100.0f);
	float aspect = ofGetWindowWidth() / static_cast<float>(ofGetWindowHeight());

	mat4 model = rotate(radians(90.0f), vec3(1, 0, 0)) * scale(vec3(0.5, 0.5, 0.5));
	mat4 view = inverse(translate(cam.pos) * rotate(radians(-45.0f), vec3(1,0,0)));
	mat4 proj = perspective(cam.fov, aspect, 0.01f, 10.0f);
	mat4 mvp = proj * view * model;

	uvShader.begin();
	uvShader.setUniformMatrix4f("mvp", mvp);
	uvShader.setUniformMatrix3f("normalMatrix", transpose(inverse(mat3(model))));
	uvShader.setUniform3f("lightDir", getLightDirection(dirLight));
	uvShader.setUniform3f("lightColor", getLightColor(dirLight));
	uvShader.setUniform3f("meshColor", vec3(1, 0, 0));
	torusMesh.draw();
	uvShader.end();
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
