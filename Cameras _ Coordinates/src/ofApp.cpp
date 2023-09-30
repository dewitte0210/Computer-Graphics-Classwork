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
	aspectRatio = static_cast<float>(ofGetWindowWidth()) / ofGetWindowHeight();

	alienImg.load("assets/walk_sheet.png");
	backgroundImg.load("assets/forest.png");
	cloudImg.load("assets/cloud.png");

	buildMesh(cloudMesh, .25, .15, glm::vec3(0.0, 0.0, 0.0));
	buildMesh(charMesh, 0.10, 0.20, glm::vec3(0.0, -0.25, 0.0));
	charPos = glm::vec3(0.0, 0.0, 0.0);

	buildMesh(backgroundMesh, 1.0, 1.0, glm::vec3(0.0, 0.0, 0.0));

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
	glm::vec2 spriteFrame = glm::vec2(static_cast<int>(frame) % 3, static_cast<int>(frame / 3));

	cam.position = glm::vec3(-1, 0, 0);
	//glm::mat4 view = buildViewMatrix(cam);
	glm::mat4 view = glm::mat4();
	glm::mat4 proj = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, 0.0f, 10.0f);
	ofDisableBlendMode();
	ofEnableDepthTest();

	animateShader.begin();
	animateShader.setUniformTexture("tex", alienImg, 0);
	animateShader.setUniform2f("size", spriteSize);
	animateShader.setUniform2f("offset", spriteFrame);
	animateShader.setUniformMatrix4f("view", view);
	animateShader.setUniformMatrix4f("proj", proj);
	animateShader.setUniformMatrix4f("model", glm::translate(charPos));
	charMesh.draw();
	animateShader.end();

	charShader.begin();
	charShader.setUniformMatrix4f("view", view);
	charShader.setUniformMatrix4f("proj", proj);
	charShader.setUniformMatrix4f("model", glm::translate(glm::vec3(0.0, 0.0, 0.0)));
	charShader.setUniformTexture("tex", backgroundImg, 0);
	backgroundMesh.draw();	
	charShader.end();

	ofDisableDepthTest();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	
	cloudShader.begin();
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ALPHA);

	static float rotation = 1.0f;
	rotation += 1.0f * ofGetLastFrameTime();

	// construct the transform for our un-rotated cloud
	glm::mat4 translationA = glm::translate(glm::vec3(-0.55, 0.0, 0.0));
	glm::mat4 scaleA = glm::scale(glm::vec3(1.5, 1.0, 1.0));
	glm::mat4 transformA = translationA * scaleA;

	//apply a rotation to that
	glm::mat4 ourRotation = glm::rotate(rotation, glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 newMatrix = translationA * ourRotation * glm::inverse(translationA);
	glm::mat4 finalMatrix = newMatrix * transformA;

	glm::mat4 transformB = buildMatrix(glm::vec3(0.4, 0.2, 0.0), 1.0f, glm::vec3(1, 1, 1));
	cloudShader.setUniformTexture("tex", cloudImg, 0);
	cloudShader.setUniformMatrix4f("model", finalMatrix);
	cloudShader.setUniformMatrix4f("view", view);
	cloudShader.setUniformMatrix4f("proj", proj);
	cloudMesh.draw();

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

