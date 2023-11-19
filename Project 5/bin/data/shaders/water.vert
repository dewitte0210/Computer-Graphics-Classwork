#version 410

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 tangent;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

uniform mat4 modelView;
uniform mat4 mvp;
uniform mat3 normalMatrix;
uniform float time;

out mat3 TBN;
out vec3 cameraSpacePos;
out vec2 fragUV;
out vec2 fragUV2;

void main(){
	gl_Position = mvp * vec4(pos, 1.0);
	vec3 T = normalize(normalMatrix * tangent);
	vec3 B = normalize(normalMatrix * cross(tangent, normal));
	vec3 N = normalize(normalMatrix * normal);
	TBN = mat3(T,B,N);
	
	float t = time * 0.05;
	float t2 = time * 0.02;
	fragUV = vec2(uv.x+t, uv.y) * 3.0;
	fragUV2 = vec2(uv.x + t2, uv.y - t2) * 2.0;
	cameraSpacePos = vec3(modelView * vec4(pos,1.0));
}