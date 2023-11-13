#version 410

layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 normal;
uniform mat4 modelView;
uniform mat4 mvp;
uniform mat3 normalMatrix;

out vec3 fragNormal;
out vec3 cameraSpacePos;

void main(){
	gl_Position = mvp * vec4(pos, 1.0);
	fragNormal = normalMatrix * (normal * -1);
	cameraSpacePos = vec3(modelView * vec4(pos,1.0));
}