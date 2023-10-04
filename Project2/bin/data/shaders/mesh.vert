#version 410

layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 normal;

uniform mat4 mvp;

out vec3 fragNormal;
void main(){
	gl_Position = mvp * vec4(pos, 1.0);
	fragNormal = normal;
}
