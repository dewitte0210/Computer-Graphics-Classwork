#version 410

uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 meshColor;

in vec3 fragNormal;
out vec4 outColor;

void main(){
	vec3 normal = normalize(fragNormal);
	float lightAmount = max(0.0, dot(normal, lightDir));
	vec3 fragLight = lightColor * lightAmount;
	outColor = vec4(meshColor * fragLight, 1.0);
}