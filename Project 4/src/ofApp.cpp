#include "ofApp.h"
#include "CameraMatrices.h"
#include "SimpleDrawNode.h"
#include "SimpleAnimationNode.h"
#include "WalkingAnimationNode.h"
#include "LitDrawNode.h"
#include "SpotLightNode.h"
#include "Lighting.h"
#include <memory>

void ofApp::updateCameraRotation(float dx, float dy) {
	dx *= ofGetLastFrameTime() * sensitivity;
	dy *= ofGetLastFrameTime() * sensitivity;
	camera.rotation *= glm::mat3(glm::rotate(dy, glm::vec3(1, 0, 0)) * glm::rotate(dx, glm::vec3(0, 1, 0)));
}
//--------------------------------------------------------------

void ofApp::setup() {
	ofDisableArbTex();
	ofEnableDepthTest();
	glEnable(GL_CULL_FACE);
	camera.position = glm::vec3(0, 0, 0);

	head.load("models/cone.ply");
	body.load("models/cylinder.ply");
	eye.load("models/sphere.ply");
	arm.load("models/cube.ply");
	wheel.load("models/torus.ply");
	robotShader.load("shaders/robot.vert", "shaders/robot.frag");

	using namespace glm;
	lighting.dirLight.direction = vec3(-1, 1, -1);
	lighting.ambientLight = vec3(0.1f);	
	lighting.pointLight.position = vec3(-1, -1, -1);
	lighting.pointLight.color = vec3(0.1, 0.5, 0.1);

	//Init scene graph
	// Creates the node that holds the center of the sceneGraph and that animated node that makes the robot move in a circle
	sceneGraphRoot.localTransform = translate(vec3(0, 0, 0));
	sceneGraphRoot.childNodes.emplace_back(new SimpleAnimationNode{ 0.5f, vec3(0,1,0) });
	sceneGraphRoot.childNodes.back()->childNodes.emplace_back(spotLightNode);
	sceneGraphRoot.childNodes.back()->childNodes.emplace_back(new SceneGraphNode{});
	std::shared_ptr<SceneGraphNode> robotRoot = sceneGraphRoot.childNodes.back()->childNodes.back();
	robotRoot->childNodes.emplace_back(new SceneGraphNode{});
	robotRoot->childNodes.back()->localTransform = translate(vec3(0, 0, -5)) * rotate(radians(-90.0f), vec3(0,1,0));
	
	//Create the head and eyes	
	robotRoot->childNodes.back()->childNodes.emplace_back(new LitDrawNode{ head, robotShader, lighting,vec3(0.1,0.1,0.5) });
	std::shared_ptr<SceneGraphNode> headNode = robotRoot->childNodes.back()->childNodes.back();
	headNode->localTransform = translate(vec3(0, 2, 0));
	headNode->childNodes.emplace_back(new LitDrawNode{ eye, robotShader, lighting, vec3(0.1,0.5,0.1)});
	headNode->childNodes.back()->localTransform = translate(vec3(0.5, 0, 0)) * scale(vec3(0.25)); //Right Eye
	headNode->childNodes.emplace_back(new LitDrawNode{ eye, robotShader, lighting, vec3(0.1,0.5,0.1)});
	headNode->childNodes.back()->localTransform = translate(vec3(-0.5, 0, 0)) * scale(vec3(0.25)); //Left Eye
	
	//Create the body
	robotRoot->childNodes.back()->childNodes.emplace_back(new LitDrawNode{ body, robotShader, lighting, vec3(0.1,0.1,0.5)});
	std::shared_ptr<SceneGraphNode> bodyNode = robotRoot->childNodes.back()->childNodes.back();
	//Create the Arms
	bodyNode->childNodes.emplace_back(new SceneGraphNode{});
	std::shared_ptr<SceneGraphNode> armsParent = bodyNode->childNodes.back();
	armsParent->localTransform = rotate(radians(-90.0f), glm::vec3(0, 1, 0));

	armsParent->childNodes.emplace_back(new WalkingAnimationNode{ 0.5f, glm::vec3(0,0,1), 0.785f}); //Pivot for Right Arm
	std::shared_ptr<SceneGraphNode> armNode = armsParent->childNodes.back();
	armNode->localTransform = translate(vec3(0, 0, 1.25));
	armNode->childNodes.emplace_back(new LitDrawNode{ arm, robotShader, lighting, vec3(0.5,0.1,0.1)});//Right Arm
	armNode->childNodes.back()->localTransform = translate(vec3(0.5,0,0)) * scale(vec3(0.75, 0.25, 0.25));

	armsParent->childNodes.emplace_back(new WalkingAnimationNode(0.5f, glm::vec3(0,0,1), 0.785f, false)); //Pivot for Left Arm
	armNode = armsParent->childNodes.back();
	armNode->localTransform = translate(vec3(0, 0, -1.25));
	armNode->childNodes.emplace_back(new LitDrawNode{ arm, robotShader, lighting, vec3(0.5,0.1,0.1)}); //Left Arm
	armNode->childNodes.back()->localTransform = translate(vec3(0.5,0,0)) * scale(vec3(0.75, 0.25, 0.25));

	//Create the wheels
	bodyNode->childNodes.emplace_back(new SceneGraphNode{});
	bodyNode->childNodes.back()->localTransform = translate(vec3(0, -2, 0));
	bodyNode->childNodes.back()->childNodes.emplace_back(new SimpleAnimationNode{ radians(90.0f), glm::vec3(1,0,0)});
	std::shared_ptr<SceneGraphNode> wheelPivot = bodyNode->childNodes.back()->childNodes.back();
	wheelPivot->localTransform = rotate(radians(90.0f), vec3(0,0,1)) ;
	wheelPivot->childNodes.emplace_back(new LitDrawNode{ wheel, robotShader, lighting, vec3(0.2,0.2,0.2)});
	wheelPivot->childNodes.back()->localTransform = translate(vec3(0, -0.5, 0)); // Right Wheel
	wheelPivot->childNodes.emplace_back(new LitDrawNode{ wheel, robotShader, lighting, vec3(0.2,0.2,0.2)});
	wheelPivot->childNodes.back()->localTransform = translate(vec3(0, 0.5, 0)); // Left Wheel
	
	spotLightNode->spotLight.cutoff = 0.98f;
	spotLightNode->spotLight.color = vec3(1.0f,0.1f,0.1f);
	lighting.spotLight = spotLightNode->spotLight;
}

//--------------------------------------------------------------
void ofApp::update(){
	if (reload) {
		robotShader.load("shaders/robot.vert", "shaders/robot.frag");
		reload = false;
	}
	velocityWorldSpace = camera.rotation * velocity * ofGetLastFrameTime();
	sceneGraphRoot.updateSceneGraph(ofGetLastFrameTime(), glm::mat4{});
}

//--------------------------------------------------------------
void ofApp::draw(){
	using namespace glm;
	float aspect = ofGetWindowWidth() / ofGetWindowHeight();
	camera.position += velocityWorldSpace * speed;
	CameraMatrices camData{ camera, aspect, 0.01f, 20.0f };
	
	mat4 model{};
	sceneGraphRoot.drawSceneGraph(camData, model);
	lighting.spotLight = spotLightNode->spotLight;
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
	if (mouseX != 0 && mouseY != 0)
	{
		updateCameraRotation(mouseX - x, mouseY - y);
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
