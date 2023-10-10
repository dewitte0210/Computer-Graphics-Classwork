#version 410

in vec3 fragNormal;

out vec4 outColor;

void main(){
	outColor = vec4(normalize(fragNormal), 1.0);
}