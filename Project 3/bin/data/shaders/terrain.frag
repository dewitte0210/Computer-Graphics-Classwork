#version 410

in vec3 fragNormal;
in vec3 cameraSpacePos;
out vec4 outColor;

void main(){
	double alpha =  smoothstep(0.0, 10.0, sqrt(pow(cameraSpacePos.x,2) + pow(cameraSpacePos.y,2) + pow(cameraSpacePos.z,2)));
	outColor = vec4(normalize(fragNormal), 1.0 - alpha);
}