#version 410

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 meshColor;
uniform vec3 ambientColor;

in vec3 fragNormal;
in vec3 cameraSpacePos;
out vec4 outColor;

void main(){
	double alpha = smoothstep(100.0, 1100.0, length(cameraSpacePos));
	vec3 normal = normalize(fragNormal);	
	float nDotL = max(0, dot(normal, lightDirection)); //calculate light intensty Cos(Theta)
	vec3 irradiance = ambientColor + lightColor * nDotL; // How much light the surface recieves
		
	outColor = vec4(pow(meshColor * irradiance, vec3(1.0/2.2)), 1.0 - alpha);
}