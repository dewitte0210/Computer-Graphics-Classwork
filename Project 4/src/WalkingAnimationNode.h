#pragma once
#include "SimpleAnimationNode.h"

class WalkingAnimationNode : public SimpleAnimationNode
{
public:
	WalkingAnimationNode(float rotationSpeed, glm::vec3 rotationAxis, float maxAngle);
	WalkingAnimationNode(float rotationSpeed, glm::vec3 rotationAxis, float maxAngle, bool swingForward);
	void updateNode(float dt, const glm::mat4& model) override;

private:
	float maxAngle; 
	bool swingForward{ true };
	float curAngle{ 0.0f };
};
