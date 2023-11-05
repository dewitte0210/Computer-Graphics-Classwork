#include "WalkingAnimationNode.h"

using namespace glm;

WalkingAnimationNode::WalkingAnimationNode(float rotationSpeed, vec3 rotationAxis, float maxAngle)
	: SimpleAnimationNode(rotationSpeed, rotationAxis), maxAngle{maxAngle}
{
}

WalkingAnimationNode::WalkingAnimationNode(float rotationSpeed, vec3 rotationAxis, float maxAngle, bool swingForward)
	: SimpleAnimationNode(rotationSpeed, rotationAxis), maxAngle{ maxAngle }, swingForward{ swingForward }
{
}

void WalkingAnimationNode::updateNode(float dt, const mat4& model) {
	if (curAngle >= maxAngle || curAngle <= -maxAngle)
	{
		swingForward = !swingForward;
	}
	float rotUpdate = swingForward ? rotationSpeed * dt : -rotationSpeed * dt;
	curAngle += rotUpdate;
	this->localTransform = rotate(rotUpdate, rotationAxis) * this->localTransform;
}