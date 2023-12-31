#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofDisableArbTex();
	ofEnableDepthTest();
	glEnable(GL_CULL_FACE);
	cam.pos = glm::vec3(0, 0, 1);
	velocity = glm::vec3(0, 0, 0);
	staff.load("assets/staff.ply");
	shapes.load("assets/stackedShapes.ply");
	staffVbo.setMesh(staff, GL_STATIC_DRAW);
	shapesVbo.setMesh(shapes, GL_STATIC_DRAW);
	meshShader.load("shaders/mesh.vert", "shaders/mesh.frag");
}

void ofApp::updateCameraRotation(float dx, float dy) {
	cameraHead += dx * ofGetLastFrameTime();
	cameraTilt += dy * ofGetLastFrameTime();
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

	// With Vbo avg fps is 47, without Vbo avg fps is 28
	meshShader.begin();
	for (int i = 0; i < 1000; i++) {
		mat4 model{ translate(vec3(0,0,-2)) * rotate(radians(-90.0f), vec3(1.0f,0.0f,0.0f)) * scale(vec3(0.08,0.08,0.08))};
		mat4 view{rotate(cameraTilt,vec3(1,0,0)) * rotate(cameraHead, vec3(0,1,0)) * translate(-cam.pos)};
		mat4 projection{ perspective(cam.fov, aspect, 0.01f, 10.0f) };
		mat4 modelView{ view * model };
		mat4 mvp{ projection * view * model };

		meshShader.setUniformMatrix4f("mvp", mvp);
		meshShader.setUniformMatrix4f("modelView", modelView);
		meshShader.setUniformMatrix3f("model", model);
		staffVbo.drawElements(GL_TRIANGLES, staffVbo.getNumIndices());
		//staff.draw();

		model = translate(vec3(-1, 0, -1)) * rotate(radians(-90.0f), vec3(1.0f, 0.0f, 0.0f)) * scale(vec3(0.25, 0.25, 0.25));
		mvp = projection * view * model;
		meshShader.setUniformMatrix4f("mvp", mvp);
		meshShader.setUniformMatrix3f("model", model);
		shapesVbo.drawElements(GL_TRIANGLES, shapesVbo.getNumIndices());
	//	shapes.draw();
	}
	meshShader.end();

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
