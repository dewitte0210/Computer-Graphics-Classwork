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
glm::mat4 buildMatrix(glm::vec3 trans, float rot, glm::vec3 scale) {
	glm::mat4 translation = glm::translate(trans);
	glm::mat4 rotation = glm::rotate(rot, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 scaler = glm::scale(scale);
	return translation * rotation * scaler;
}

glm::mat4 buildViewMatrix(CameraData cam) {
	return glm::inverse(buildMatrix(cam.position, cam.rotation, glm::vec3(1, 1, 1)));
}
//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();
	ofEnableDepthTest();
	charPos = glm::vec3(0.0, 0.0, 0.0);
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
	if (walkRight)
	{
		float speed = 0.4 * ofGetLastFrameTime();
		charPos += glm::vec3(speed, 0, 0);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	static float frame = 0.0;
	frame = (frame > 10) ? 0.0 : frame += 0.2;
	glm::vec2 spriteSize = glm::vec2(0.28, 0.19);
	glm::vec2 spriteFrame = glm::vec2((int)frame % 3, static_cast<int>(frame / 3));

	cam.position = glm::vec3(-1, 0, 0);
	glm::mat4 view = buildViewMatrix(cam);

	ofDisableBlendMode();
	ofEnableDepthTest();

	animateShader.begin();
	animateShader.setUniformTexture("tex", alienImg, 0);
	animateShader.setUniform2f("size", spriteSize);
	animateShader.setUniform2f("offset", spriteFrame);
	animateShader.setUniformMatrix4f("view", view);
	animateShader.setUniformMatrix4f("model", glm::translate(charPos));
	charMesh.draw();
	animateShader.end();

	charShader.begin();
	charShader.setUniformMatrix4f("view", view);
	charShader.setUniformMatrix4f("model", glm::translate(glm::vec3(0.0, 0.0, 0.0)));
	charShader.setUniformTexture("tex", backgroundImg, 0);
	backgroundMesh.draw();	
	charShader.end();

	ofDisableDepthTest();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	
	cloudShader.begin();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ALPHA);

	static float rotation = 0.0f;
	rotation += 0.1f;

	// construct the transform for our un-rotated cloud
	glm::mat4 translationA = glm::translate(glm::vec3(-0.55, 0.0, 0.0));
	glm::mat4 scaleA = glm::scale(glm::vec3(1.5, 1, 1));
	glm::mat4 transformA = translationA * scaleA;

	//apply a rotation to that
	glm::mat4 ourRotation = glm::rotate(rotation, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 newMatrix = translationA * ourRotation * glm::inverse(translationA);
	glm::mat4 finalMatrix = newMatrix * transformA;

	glm::mat4 transformB = buildMatrix(glm::vec3(0.4, 0.2, 0.0), 1.0f, glm::vec3(1, 1, 1));
	cloudShader.setUniformTexture("tex", cloudImg, 1);
	cloudShader.setUniformMatrix4f("transform", finalMatrix);
	cloudShader.setUniformMatrix4f("model", finalMatrix);
	cloudShader.setUniformMatrix4f("view", view);
	cloudMesh.draw();

	cloudShader.setUniformMatrix4f("transform", transformB);
	cloudShader.setUniformMatrix4f("model", transformB);
	cloudMesh.draw();
	cloudShader.end();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ofKey::OF_KEY_RIGHT)
	{
		walkRight = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == ofKey::OF_KEY_RIGHT)
	{
		walkRight = false;
	}
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

