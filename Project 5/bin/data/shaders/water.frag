#version 410

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 meshColor;
uniform vec3 ambientColor;
uniform float fogStart;
uniform float fogEnd;
uniform sampler2D normalMap;

in mat3 TBN;
in vec3 cameraSpacePos;
in vec2 fragUV;

out vec4 outColor;
void main(){
		
	double alpha = smoothstep(fogStart, fogEnd, length(cameraSpacePos));
	vec3 normal = texture(normalMap, fragUV).rgb;
	normal = normalize(TBN * (normal * 2.0 - 1.0));

	float nDotL = max(0, dot(normal, lightDirection)); //calculate light intensty Cos(Theta)
	vec3 irradiance = ambientColor + (lightColor * nDotL); // How much light the surface recieves
		
	outColor = vec4(pow(meshColor * irradiance, vec3(1.0/2.2)), 1.0 - alpha);
}