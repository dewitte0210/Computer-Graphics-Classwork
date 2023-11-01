#version 410
layout (location = 0) in vec3 pos;
layout (location = 2) in vec3 normal;

uniform mat4 mvp;
uniform mat3 normalMatrix;
uniform mat4 model;
out vec3 fragNormal;
out vec3 worldPosition;
void main(){
    gl_Position = mvp * vec4(pos,1.0);
    worldPosition = (model * vec4(pos, 1.0)).xyz; 
    fragNormal = normalMatrix * normalize(normal);
}