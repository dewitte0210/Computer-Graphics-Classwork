#version 410

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 meshColor;
uniform float fogStart;
uniform float fogEnd;
uniform sampler2D normalMap;
uniform samplerCube envMap;
in mat3 TBN;
in vec3 cameraSpacePos;
in vec2 fragUV;
in vec2 fragUV2;

out vec4 outColor;
void main(){
		
	double alpha = smoothstep(fogStart, fogEnd, length(cameraSpacePos));
	
	vec3 normal = texture(normalMap, fragUV).rgb;
	normal = (normal * 2.0 - 1.0);
	vec3 normal2 = texture(normalMap, fragUV2).rgb;
	normal2 = (normal2 * 2.0 - 1.0);
	normal = normalize(TBN * (normal + normal2));

	float nDotL = max(0, dot(normal, lightDirection)); //calculate light intensty Cos(Theta)
	vec3 envIrradiance = pow(texture(envMap, normal).rgb, vec3(2.2));
	vec3 irradiance = envIrradiance + (lightColor * nDotL); // How much light the surface recieves
		
	outColor = vec4(pow(meshColor * irradiance, vec3(1.0/2.2)), min(0.9, 1.0 - alpha));
}