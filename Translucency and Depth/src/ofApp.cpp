#include "ofApp.h"

void buildMesh(ofMesh& mesh, float w, float h, glm::vec3 pos) {
	float verts[] = { -w + pos.x, -h + pos.y, pos.z,
		-w + pos.x, h + pos.y, pos.z,
		w + pos.x, h + pos.y, pos.z,
		w + pos.x, -h + pos.y, pos.z };

	float uvs[] = { 0,0,0,1,1,1,1,0 };

	for (int i = 0; i < 4; ++i)
	{
		int idx = i * 3;
		int uvIdx = i * 2;
		mesh.addVertex(glm::vec3(verts[idx], verts[idx + 1], verts[idx + 2]));
		mesh.addTexCoord(glm::vec2(uvs[uvIdx], uvs[uvIdx + 1]));
	}
	ofIndexType indicies[6] = { 0,1,2,2,3,0 };
	mesh.addIndices(indicies, 6);
}


//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();
	ofEnableDepthTest();

	alienImg.load("assets/walk_sheet.png");
	backgroundImg.load("assets/forest.png");
	cloudImg.load("assets/cloud.png");
	sunImg.load("assets/sun.png");

	buildMesh(cloudMesh, .25, .16, glm::vec3(-0.55, 0, 0));
	buildMesh(charMesh, 0.10, 0.2, glm::vec3(0.0, -0.25, 0.0));
	buildMesh(backgroundMesh, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.5));
	buildMesh(sunMesh, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.4));

	charShader.load("shaders/alien.vert", "shaders/alphaTest.frag");
	cloudShader.load("shaders/alien.vert", "shaders/cloud.frag");
	animateShader.load("shaders/spriteSheet.vert", "shaders/alphaTest.frag");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	static float frame = 0.0;
	frame = (frame > 10) ? 0.0 : frame += 0.2;
	glm::vec2 spriteSize = glm::vec2(0.28, 0.19);
	glm::vec2 spriteFrame = glm::vec2((int)frame % 3, static_cast<int>(frame / 3));
	ofDisableBlendMode();
	ofEnableDepthTest();

	animateShader.begin();
	animateShader.setUniformTexture("tex", alienImg, 0);
	animateShader.setUniform2f("size", spriteSize);
	animateShader.setUniform2f("offset", spriteFrame);
	charMesh.draw();
	animateShader.end();

	charShader.begin();
	charShader.setUniformTexture("tex", backgroundImg, 0);
	backgroundMesh.draw();	
	charShader.end();

	ofDisableDepthTest();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	cloudShader.begin();	
	charShader.setUniformTexture("tex", sunImg, 0);
	sunMesh.draw();
	
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ALPHA);
	cloudShader.setUniformTexture("tex", cloudImg, 1);
	cloudMesh.draw();
	cloudShader.end();

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

