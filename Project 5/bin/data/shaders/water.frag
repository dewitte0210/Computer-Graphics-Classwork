#version 410

uniform vec3 dirLightDir;
uniform vec3 dirLightColor;
uniform vec3 meshColor;
uniform vec3 cameraPos;
uniform vec3 specularColor;
uniform vec3 pointLightPos;
uniform vec3 pointLightColor;

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
	//Environment Ambience
	vec3 envIrradiance = pow(texture(envMap, normal).rgb, vec3(2.2));
	
	//Point Light
	vec3 pointLightDir = normalize(fragWorldPos - pointLightPos);
	float pointNDotL = max(0, dot(normal, pointLightDir));
	float falloff = 1.0 / dot(pointLightDir,pointLightDir);
	vec3 pointLight = pointNDotL * pointLightColor * falloff;
	
	// Calculations for specular reflections
	vec3 view = normalize(cameraPos - fragWorldPos);
	vec3 envLightDir = reflect(-view, normal);
	vec3 envReflection = (pow(texture(reflectMap, envLightDir).rgb, vec3(2.2)));
	
	//	 Fresnel Effect calculations
	vec3 halfway = normalize(envLightDir + view);
	float hDotL = max(0.0, dot(halfway, envLightDir));
	vec3 fresnel = mix(specularColor,vec3(1), pow(1 - hDotL,5));

	vec3 specularReflection = fresnel * envReflection ;

	//Calculations for Specular Highlights
	//Directional Highlights
	vec3 dirHalfway = normalize(dirLightDir + view);
	float dirHDotL = max(0.0, dot(halfway, dirLightDir));
	vec3 dirFresnel = mix(dirLightColor,vec3(1), pow(1 - dirHDotL, 5));
	
	float nDotH = dot(dirHalfway, normal);
	float D = pow(m,2) / pow(mix(1, pow(m,2), pow(nDotH,2)),2);
	
	float uV = dot(normal, view);
	float uL = dot(normal, dirLightDir);
	float G = 0.5 / mix(2 * uL * uV, uL + uV, pow(m,2));

	vec3 directionalHighlight = D * G * dirFresnel * uL * dirLightColor;	


	//Pont Light Highlights
	vec3 pointHalfway = normalize(pointLightDir + view);
	float pointHDotL = dot(pointHalfway, pointLightDir);
	vec3 pointFresnel = mix(pointLightColor, vec3(1), pow(1 - pointHDotL, 5));

	nDotH = dot(pointHalfway, normal);
	D = pow(m,2) / pow(mix(1, pow(m,2), pow(nDotH,2)),2);

	G = 0.5 / mix(2 * uL * uV, uL + uV, pow(m,2));

	vec3 pointHighlight = D * G * pointFresnel * uL * pointLightColor;

	vec3 specularHighlight = directionalHighlight * pointHighlight;
	vec3 specular = specularHighlight + specularReflection;
	outColor = vec4(pow(specular  , vec3(1.0/2.2)), min(0.9, 1.0 - alpha));
}
