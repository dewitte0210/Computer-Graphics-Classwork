#include "ofApp.h"
#include "CameraMatrices.h"
#include "SimpleDrawNode.h"
#include "SimpleAnimationNode.h"
#include <memory>

void ofApp::updateCameraRotation(float dx, float dy) {
	dx *= ofGetLastFrameTime() * sensitivity;
	dy *= ofGetLastFrameTime() * sensitivity;
	camera.rotation = camera.rotation * glm::mat3(glm::rotate(dx, glm::vec3(1, 0, 0)) * glm::rotate(dy, glm::vec3(0, 1, 0)));
}
//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();
	camera.position = glm::vec3(0, 0, 2);
//	camera.rotation = glm::mat3(rotate(0.0f, glm::vec3(0,0,0)));

	head.load("models/cone.ply");
	body.load("models/cylinder.ply");
	eye.load("models/sphere.ply");
	arm.load("models/cube.ply");
	wheel.load("models/torus.ply");
	robotShader.load("shaders/robot.vert", "shaders/robot.frag");
	
	//Init scene graph
	using namespace glm;

	//Create the Head and Eyes
	sceneGraphRoot.localTransform = translate(vec3(0, 0, -2));
	sceneGraphRoot.childNodes.emplace_back(new SimpleDrawNode{ head, robotShader }); // Create Head
	std::shared_ptr<SceneGraphNode> headNode = sceneGraphRoot.childNodes.back();
	headNode->localTransform = translate(vec3(0, 1, 0));
	headNode->childNodes.emplace_back(new SimpleDrawNode{ eye, robotShader });
	headNode->childNodes.back()->localTransform = translate(vec3(0.25, 0, 0)); //Right Eye
	headNode->childNodes.emplace_back(new SimpleDrawNode{ eye, robotShader });
	headNode->childNodes.back()->localTransform = translate(vec3(-0.25, 0, 0)); //Left Eye
	
	//Create the body
	sceneGraphRoot.childNodes.emplace_back(new SimpleDrawNode{ body, robotShader });
	std::shared_ptr<SceneGraphNode> bodyNode = sceneGraphRoot.childNodes.back();

	//Create the Arms
	bodyNode->childNodes.emplace_back(new SimpleAnimationNode{ 1.0f, glm::vec3(0,1,0) });
	std::shared_ptr<SceneGraphNode> armNode = bodyNode->childNodes.back();
	armNode->localTransform = translate(vec3(1, 0, 0));
	armNode->childNodes.emplace_back(new SimpleDrawNode{ arm, robotShader });
	bodyNode->childNodes.emplace_back(new SimpleAnimationNode(1.0f, glm::vec3(0, 1, 0)));
	armNode = bodyNode->childNodes.back();
	armNode->localTransform = translate(vec3(-1, 0, 0));
	armNode->childNodes.emplace_back(new SimpleDrawNode{ arm, robotShader });

	//Create the wheels
	bodyNode->childNodes.emplace_back(new SimpleAnimationNode{ 2.0f, glm::vec3(0,1,0) });
	std::shared_ptr<SceneGraphNode> wheelPivot = bodyNode->childNodes.back();
	wheelPivot->localTransform = translate(vec3(0, -1, 0));
	wheelPivot->childNodes.emplace_back(new SimpleDrawNode{ wheel, robotShader });
	wheelPivot->childNodes.back()->localTransform = translate(vec3(0.5, 0, 0)); // Right Wheel
	wheelPivot->childNodes.emplace_back(new SimpleDrawNode{ wheel, robotShader });
	wheelPivot->childNodes.back()->localTransform = translate(vec3(-0.5, 0, 0)); // Left Wheel
}

//--------------------------------------------------------------
void ofApp::update(){
	if (reload) {
		robotShader.load("shaders/robot.vert", "shaders/robot.frag");
		reload = false;
	}
	velocityWorldSpace = camera.rotation * velocity * ofGetLastFrameTime();
}

//--------------------------------------------------------------
void ofApp::draw(){
	using namespace glm;
	float aspect = ofGetWindowWidth() / ofGetWindowHeight();
	camera.position += velocityWorldSpace * speed;
	CameraMatrices camData{ camera, aspect, 0.01f, 20.0f };
	
	mat4 model{};
	sceneGraphRoot.drawSceneGraph(camData, model);
	sceneGraphRoot.updateSceneGraph(ofGetLastFrameTime(), model);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ')
	{
		velocity.y = 1;
	}else if (key == OF_KEY_LEFT_CONTROL)
	{
		velocity.y = -1;
	}else if (key == 'd')
	{
		velocity.x = 1;
	}else if (key == 'a')
	{
		velocity.x = -1;
	}else if (key == 'w')
	{
		velocity.z = -1;
	}else if (key == 's')
	{
		velocity.z = 1;
	}
	else if (key == 'p')
	{
		reload = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	if (key == OF_KEY_LEFT_CONTROL || key == ' ')
	{
		velocity.y = 0;
	}else if (key == 'd' || key == 'a')
	{
		velocity.x = 0;
	}else if (key == 'w' || key == 's')
	{
		velocity.z = 0;
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
