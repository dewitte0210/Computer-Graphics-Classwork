#version 410

out vec4 outColor;
in vec3 fragNormal;

void main(){
	outColor = vec4(fragNormal,1);

}