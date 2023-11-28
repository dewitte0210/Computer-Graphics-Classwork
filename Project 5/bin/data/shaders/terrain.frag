#version 410

uniform vec3 dirLightDir;
uniform vec3 dirLightColor;
uniform vec3 pointLightPos;
uniform vec3 pointLightColor;

uniform float fogStart;
uniform float fogEnd;
uniform sampler2D tex;
uniform sampler2D normalMap;
uniform samplerCube envMap;

in mat3 TBN;
in vec3 cameraSpacePos;
in vec3 fragWorldPos;
in vec2 fragUV;
out vec4 outColor;

void main(){
	double alpha = smoothstep(fogStart, fogEnd, length(cameraSpacePos));
	vec3 normal = texture(normalMap, fragUV).rgb;	
	normal = normalize(TBN * (normal * 2.0 - 1.0));
	
	//Directional Light
	float nDotL = max(0, dot(normal, dirLightDir)); //calculate light intensty Cos(Theta)
	vec3 directionalLight = nDotL * dirLightColor;

	//PointLight
	vec3 toPointLight = fragWorldPos - pointLightPos;
	vec3 pointLightDir = normalize(toPointLight);
	float pointNDotL = max(0, dot(normal, normalize(pointLightDir)));
	float falloff = 1.0 / dot(toPointLight,toPointLight);
	vec3 pointLight = pointNDotL * pointLightColor;

	vec3 envIrradiance = pow(texture(envMap, normal).rgb, vec3(2.2));
	vec3 irradiance = envIrradiance + directionalLight + pointLight; // How much light the surface recieves
	
	//Final color calculations	
	vec3 textureColor = vec3(pow(texture(tex, fragUV), vec4(2.2)));	
	outColor = vec4( pow(textureColor * irradiance, vec3(1.0/2.2)), 1.0 - alpha);
}