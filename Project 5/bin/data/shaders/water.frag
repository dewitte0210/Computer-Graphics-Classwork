#version 410

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 meshColor;
uniform vec3 cameraPos;
uniform vec3 specularColor;

uniform float fogStart;
uniform float fogEnd;
uniform sampler2D normalMap;
uniform samplerCube envMap;
uniform samplerCube reflectMap;

in mat3 TBN;
in vec3 cameraSpacePos;
in vec2 fragUV;
in vec2 fragUV2;
in vec3 fragWorldPos;
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
	
	vec3 diffuse = meshColor * irradiance;
	// Calculations for specular reflections
	vec3 view = cameraPos - fragWorldPos;
	vec3 envLightDir = reflect(-view, normal);
	vec3 specularReflection = (pow(texture(reflectMap, envLightDir).rgb, vec3(2.2)) * specularColor);

	outColor = vec4(pow(specularReflection + diffuse, vec3(1.0/2.2)), min(0.9, 1.0 - alpha));
}