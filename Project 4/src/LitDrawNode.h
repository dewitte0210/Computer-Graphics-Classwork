#pragma once
#include "SimpleDrawNode.h"
#include "Lighting.h"

class LitDrawNode : public SimpleDrawNode
{
public:
    glm::vec3 meshColor { glm::vec3(1) };
    const Lighting& sceneLighting;
    // Constructor
    LitDrawNode(const ofMesh& mesh, const ofShader& shader, const Lighting& sceneLighting, const glm::vec3 mColor);

    // Override the base class's draw function
    void drawNode(const CameraMatrices& camera, const glm::mat4& model) const override;

};