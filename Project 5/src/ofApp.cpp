#include "ofApp.h"
#include "buildTerrainMesh.h"
#include "CameraMatrices.h"
#include "calcTangents.h"

// TODO: Highlights from all sources
// Fresnel effect using Schlick's approximation
// Cook - Torrance model using GGX for distribution, Smith for masking, and Schlick's approximation
void ofApp::updateCameraRotation(float dx, float dy) {
	dx *= ofGetLastFrameTime();
	dy *= ofGetLastFrameTime();
	camera.rotation *= glm::mat3(glm::rotate(dy, glm::vec3(1, 0, 0)) * glm::rotate(dx, glm::vec3(0, 1, 0)));
}
void ofApp::reloadShaders() {
	terrainShader.load("shaders/terrain.vert", "shaders/terrain.frag");
	skyboxShader.load("shaders/skybox.vert", "shaders/skybox.frag");
	waterShader.load("shaders/water.vert", "shaders/water.frag");
	needsReload = false;
}
void setupTexture(ofImage& tex, string filepath) {
	tex.load(filepath);
	tex.getTexture().generateMipmap();
	tex.getTexture().setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	tex.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
}
void ofApp::drawCube(const CameraMatrices& camera) {
	glDisable(GL_CULL_FACE);
	skyboxShader.begin();
	glDepthFunc(GL_LEQUAL);
	skyboxShader.setUniformMatrix4f("mvp", camera.getProj() * glm::mat4(glm::mat3(camera.getView())));
	skyboxShader.setUniformTexture("cubeMap", cubeMap.getTexture(), 0);
	cubeMesh.draw();
	skyboxShader.end();
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
}
//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();
	ofEnableDepthTest();
	ofSetBackgroundColor(53,81,92);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	velocity = glm::vec3(0, 0, 0);
	mainLight.direction = glm::vec3(1, -1, 1);
	mainLight.lightColor = glm::vec3(1);

	heightmap.setUseTexture(false);
	heightmap.load("textures/TamrielLowRes.png");
	assert(heightmap.getWidth() != 0 && heightmap.getHeight() != 0);

	highResHeightmap.setUseTexture(false);
	highResHeightmap.load("textures/TamrielHighRes.png");
	assert(highResHeightmap.getWidth() != 0 && highResHeightmap.getHeight() != 0);

	float scale{ (highResHeightmap.getWidth() - 1) / (heightmap.getWidth() - 1) };
	
	//Create Water Plane
	buildWaterMesh(water, 0, 0, heightmap.getWidth() - 1, heightmap.getHeight() - 1, WATER_HEIGHT, glm::vec3(scale, 1, scale));
	calcTangents(water);

	buildTerrainMesh(terrain, heightmap, 0, 0, heightmap.getWidth() - 1, heightmap.getHeight() - 1, glm::vec3(scale, 50 * scale, scale));
	calcTangents(terrain);
	camera.position = glm::vec3((highResHeightmap.getWidth() - 1) * 0.5f, 820, (highResHeightmap.getHeight() - 1) * 0.5f);
	cellManager.initializeForPosition(camera.position);

	// load Skybox mesh and Cubemap images
	cubeMesh.load("models/cube.ply");
	cubeMap.load("textures/skybox_front.png", "textures/skybox_back.png", "textures/skybox_right.png",
		"textures/skybox_left.png", "textures/skybox_top.png", "textures/skybox_bottom.png");
	cubeMap.getTexture().generateMipmap();
	irradianceMap.load("textures/irradiance_front.png", "textures/irradiance_back.png", "textures/irradiance_right.png", "textures/irradiance_left.png",
		"textures/irradiance_top.png", "textures/irradiance_bottom.png");
	irradianceMap.getTexture().generateMipmap();

	skyboxShader.load("shaders/skybox.vert", "shaders/skybox.frag");
	waterShader.load("shaders/water.vert", "shaders/water.frag");
	terrainShader.load("shaders/terrain.vert", "shaders/terrain.frag");

	setupTexture(terrainTex, "textures/aerial_grass_rock_diff_4k.png");
	setupTexture(terrainNrml, "textures/aerial_grass_rock_nor_4k.png");
	setupTexture(waterNrml, "textures/water_nrm.png");
}

//--------------------------------------------------------------
void ofApp::update(){
	velocityWorldSpace = camera.rotation * velocity * ofGetLastFrameTime();
	cellManager.optimizeForPosition(camera.position);
	if (needsReload)
	{
		reloadShaders();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	using namespace glm;
	t += ofGetLastFrameTime();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	camera.fov = radians(100.0f);
	float aspect = static_cast<float>(ofGetWindowWidth()) / ofGetWindowHeight();
	camera.position += velocityWorldSpace * speed;
	CameraMatrices camData{ camera, aspect, 0.01f, 5000.0f };
	drawCube(camData);
	
	//draw low LOD terrain in the background
	mat4 projection{ perspective(radians(90.0f), aspect, 200.0f, 5000.0f)};
	mat4 model{ mat4()};
	mat4 mvp{ projection * camData.getView() * model };

	waterShader.begin();
	waterShader.setUniformMatrix4f("modelView", camData.getView() * model);
	waterShader.setUniformMatrix4f("mvp", mvp);
	waterShader.setUniformMatrix3f("normalMatrix", model);
	waterShader.setUniformMatrix4f("model", model);
	waterShader.setUniform3f("lightDirection", mainLight.direction);
	waterShader.setUniform3f("lightColor", mainLight.lightColor);
	waterShader.setUniform3f("meshColor", WATER_COLOR);
	waterShader.setUniform3f("camerPos", camera.position);
	waterShader.setUniform3f("specularColor", SPECULAR_COLOR);
	waterShader.setUniform1f("fogStart", 500.0f);
	waterShader.setUniform1f("fogEnd", 5000.0f);
	waterShader.setUniform1f("time", t);	
	waterShader.setUniform1f("m", 0.5);
	waterShader.setUniformTexture("normalMap", waterNrml.getTexture(), 0);
	waterShader.setUniformTexture("envMap", irradianceMap.getTexture(), 1);
	waterShader.setUniformTexture("reflectMap", cubeMap.getTexture(), 2);
	water.draw();
	waterShader.end();

	terrainShader.begin();
	terrainShader.setUniform1f("fogStart", 500.0f);
	terrainShader.setUniform1f("fogEnd", 5000.0f);
	terrainShader.setUniform3f("lightDirection", mainLight.direction);
	terrainShader.setUniform3f("lightColor", mainLight.lightColor);
	terrainShader.setUniformMatrix4f("mvp", mvp);
	terrainShader.setUniformMatrix3f("normalMatrix", model);
	terrainShader.setUniformMatrix4f("modelView", camData.getView() * model);
	terrainShader.setUniformTexture("tex", terrainTex.getTexture(), 0);
	terrainShader.setUniformTexture("normalMap", terrainNrml.getTexture(), 1);
	terrainShader.setUniformTexture("envMap", irradianceMap.getTexture(), 2);
	terrain.draw();

	//switch to high LOD for closer terrain
	glClear(GL_DEPTH_BUFFER_BIT);
    projection = perspective(radians(90.0f), aspect, 0.5f, 500.0f);
	mvp = projection * camData.getView() * model;
	terrainShader.setUniformMatrix4f("mvp", mvp);
	terrainShader.setUniform1f("fogStart", 400.0f);
	terrainShader.setUniform1f("fogEnd", 500.0f);
	cellManager.drawActiveCells(camera.position, 1000.0f);
	terrainShader.end();
	
	waterShader.begin();
	waterShader.setUniformMatrix4f("modelView", camData.getView() * model);
	waterShader.setUniformMatrix4f("mvp", mvp);
	waterShader.setUniformMatrix3f("normalMatrix", model);
	waterShader.setUniformMatrix4f("model", model);
	waterShader.setUniform3f("lightDirection", mainLight.direction);
	waterShader.setUniform3f("lightColor", mainLight.lightColor);
	waterShader.setUniform3f("meshColor", WATER_COLOR);
	waterShader.setUniform3f("cameraPos", camera.position);
	waterShader.setUniform3f("specularColor", SPECULAR_COLOR);
	waterShader.setUniform1f("fogStart", 400.0f);
	waterShader.setUniform1f("fogEnd", 500.0f);
	waterShader.setUniform1f("time", t);
	waterShader.setUniform1f("m", 0.5);
	waterShader.setUniformTexture("normalMap", waterNrml.getTexture(), 0);
	waterShader.setUniformTexture("envMap", irradianceMap.getTexture(), 1);
	waterShader.setUniformTexture("reflectMap", cubeMap.getTexture(), 2);
	water.draw();
	waterShader.end();
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
		speed = 500;
	}
	else if (key == 'r') 
	{
		needsReload = true;
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
		speed = 150;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	if (mouseX != 0 && mouseY != 0) {
		updateCameraRotation(sensitivity * (mouseX - x), sensitivity * (mouseY - y));
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
