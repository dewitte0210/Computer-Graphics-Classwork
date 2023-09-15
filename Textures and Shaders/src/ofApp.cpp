#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofDisableArbTex();
	img.load("mandril_color.tif");
	img.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	img2.load("berries.png");
	img2.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);

	quad.addVertex(glm::vec3(-1.0f, 1.0f, 0.0f));
	quad.addVertex(glm::vec3(-1.0f, -1.0f, 0.0f));
	quad.addVertex(glm::vec3(1.0f, 1.0f, 0.0f));	
	quad.addVertex(glm::vec3(1.0f, -1.0f, 0.0f));

	quad.addColor(ofFloatColor(1.0f, 0.0f, 0.0f, 1.0f));
	quad.addColor(ofFloatColor(0.0f, 1.0f, 0.0f, 1.0f));
	quad.addColor(ofFloatColor(0.0f, 0.0f, 1.0f, 1.0f));
	quad.addColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f)); 

	quad.addTexCoord(glm::vec2(0, 1));
	quad.addTexCoord(glm::vec2(0, 0));
	quad.addTexCoord(glm::vec2(1, 1));
	quad.addTexCoord(glm::vec2(1, 0));

	ofIndexType indicies[6] = { 0,1,2,1,2,3 };
	quad.addIndices(indicies, 6);
	shader.load("shaders/uvPassthrough.vert", "shaders/blendTex.frag");

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	shader.begin();
	shader.setUniformTexture("mandrilTex", img, 0);
	shader.setUniformTexture("berriesTex", img2, 1);
	shader.setUniform1f("time", ofGetElapsedTimef());
	shader.setUniform1f("brightness", brightness );
	shader.setUniform1f("multiply", multiply);
	shader.setUniform1f("add", add);
	quad.draw();
	shader.end();
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
