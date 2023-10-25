#version 410

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 meshColor;
uniform vec3 ambientColor;

in vec3 fragNormal;
in vec3 cameraSpacePos;
out vec4 outColor;

void main(){
	float alpha = smoothstep(400, 1000, length(cameraSpacePos));
	vec3 normal = normalize(fragNormal);	
	float nDotL = max(0, dot(normal, lightDirection)); //calculate light intensty Cos(Theta)
	vec3 irradiance = ambientColor + lightColor * nDotL; // How much light the surface recieves
		
	outColor = vec4(pow(meshColor * irradiance, vec3(1.0/2.2)), alpha);
}