#version 410

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 meshColor;
uniform vec3 cameraPos;
uniform vec3 specularColor;

uniform float fogStart;
uniform float fogEnd;
uniform float m;
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
	vec3 view = normalize(cameraPos - fragWorldPos);
	vec3 envLightDir = reflect(-view, normal);
	vec3 envReflection = (pow(texture(reflectMap, envLightDir).rgb, vec3(2.2)) * specularColor);
	
	//	 Fresnel Effect calculations
	vec3 halfway = normalize(envLightDir + view);
	float hDotL = max(0.0, dot(halfway, envLightDir));
	vec3 fresnel = mix(specularColor,vec3(1), pow(1 - hDotL,5));

	vec3 specularReflection = fresnel * envReflection;
	
	//Calculations for Specular Highlights
	halfway = normalize(lightDirection + view);
	hDotL = max(0.0, dot(halfway, lightDirection));
	fresnel = mix(lightColor,vec3(1), pow(1 - hDotL, 5));
	
	float nDotH = dot(halfway, normal);
	float D = pow(m,2) / pow(mix(1, pow(m,2), pow(nDotH,2)),2);
	
	float uV = dot(normal, view);
	float uL = dot(normal, lightDirection);
	float G = 0.5 / mix(2 * uL * uV, uL + uV, pow(m,2));
	
	vec3 specularHighlight = D * G * fresnel;
	outColor = vec4(pow(specularReflection + diffuse + specularHighlight , vec3(1.0/2.2)), min(0.9, 1.0 - alpha));
}