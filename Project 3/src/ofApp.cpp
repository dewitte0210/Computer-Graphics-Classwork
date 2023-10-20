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
	velocity = glm::vec3(0, 0, 0);
	mainLight.direction = glm::vec3(-1, 1, 1);
	mainLight.lightColor = glm::vec3(1);

	heightmap.setUseTexture(false);
	heightmap.load("assets/TamrielLowRes.png");
	assert(heightmap.getWidth() != 0 && heightmap.getHeight() != 0);

	highResHeightmap.setUseTexture(false);
	highResHeightmap.load("assets/TamrielHighRes.png");
	assert(highResHeightmap.getWidth() != 0 && highResHeightmap.getHeight() != 0);
	
	float scale{ highResHeightmap.getWidth() / heightmap.getWidth() };
	buildTerrainMesh(terrain, heightmap, 0, 0, heightmap.getWidth() - 1, heightmap.getHeight() - 1, glm::vec3(scale, 50 * scale, scale));
	terrain.flatNormals();
	
	cam.pos = glm::vec3((highResHeightmap.getWidth() - 1) * 0.5f, 720, (highResHeightmap.getHeight() - 1) * 0.5f);
	cellManager.initializeForPosition(cam.pos);
	terrainShader.load("shaders/terrain.vert", "shaders/terrain.frag");
}

//--------------------------------------------------------------
void ofApp::update(){
	velocityWorldSpace = glm::mat3(glm::rotate(-cameraHead, glm::vec3(0, 1, 0))) * velocity * ofGetLastFrameTime();
	cellManager.optimizeForPosition(cam.pos);
}

//--------------------------------------------------------------
void ofApp::draw(){
	using namespace glm;

	cam.fov = radians(100.0f);
	float aspect = static_cast<float>(ofGetWindowWidth()) / ofGetWindowHeight();
	cam.pos += velocityWorldSpace * speed;

	mat4 view{ rotate(cameraTilt, vec3(1,0,0)) * rotate(cameraHead, vec3(0,1,0)) * translate(-cam.pos)};

	terrainShader.begin();
	//draw low LOD terrain in the background	
	mat4 projection{ perspective(radians(100.0f), aspect, 1000.0f, 10000.0f)};
	mat4 model{ mat4() * translate(vec3(0,-800,0)) };
	mat4 mvp = projection * view * model;
	terrainShader.setUniform3f("meshColor", vec3(0.2, 1, 0.4));
	terrainShader.setUniform3f("lightDirection", mainLight.direction);
	terrainShader.setUniform3f("lightColor", normalize(mainLight.lightColor));
	terrainShader.setUniformMatrix4f("mvp", mvp);
	terrainShader.setUniformMatrix3f("normalMatrix", model);
	terrainShader.setUniformMatrix4f("modelView", view * model);
	terrain.draw();

	//switch to high LOD for closer terrain
	glClear(GL_DEPTH_BUFFER_BIT);
	projection = perspective(radians(100.0f), aspect, 0.01f, 1000.0f);
	mvp = projection * view * model;
	terrainShader.setUniformMatrix4f("mvp", mvp);
	cellManager.drawActiveCells(cam.pos, 1000.0f);
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
	else if (key == OF_KEY_LEFT_SHIFT)
	{
		speed = 960;
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
	else if (key == OF_KEY_LEFT_SHIFT)
	{
		speed = 320;
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

void ofApp::exit() {
	cellManager.stop();
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
