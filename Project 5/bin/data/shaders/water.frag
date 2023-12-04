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

	//Directional Light	
	float nDotL = max(0, dot(normal, normalize(dirLightDir))); //calculate light intensty Cos(Theta)
	vec3 dirLight = nDotL * dirLightColor;

	//Environment Ambience
	vec3 envIrradiance = pow(texture(envMap, normal).rgb, vec3(2.2));
	
	//Point Light
	vec3 toPointLight = pointLightPos - fragWorldPos;
	vec3 pointLightDir = normalize(toPointLight);
	float pointNDotL = max(0, dot(normal, pointLightDir));
	float falloff = 1.0 / dot(toPointLight,toPointLight);
	vec3 pointLight = pointNDotL * pointLightColor * falloff;

	vec3 irradiance = envIrradiance + dirLight + pointLight; // How much light the surface recieves
	vec3 diffuse = irradiance;

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
	float dirHDotL = max(0.0, dot(dirHalfway, dirLightDir));
	vec3 dirFresnel = mix(dirLightColor,vec3(1), pow(1 - dirHDotL, 5));
	
	float dirNDotH = dot(dirHalfway, normal);
	float dirD = pow(m,2) / pow(mix(1, pow(m,2), pow(dirNDotH,2)),2);
	
	float dirUv = dot(normal, view);
	float dirUl = dot(normal, dirLightDir);
	float dirG = 0.5 / mix(2 * dirUl * dirUv, dirUl + dirUv, pow(m,2));

	vec3 directionalHighlight = dirD * dirG * fresnel;	


	//Pont Light Highlights
	vec3 pointHalfway = normalize(pointLightDir + view);
	float pointHDotL = dot(pointHalfway, pointLightDir);
	vec3 pointFresnel = mix(pointLightColor, vec3(1), pow(1 - pointHDotL, 5));

	float pointNDotH = dot(pointHalfway, normal);
	float pointD = pow(m,2) / pow(mix(1, pow(m,2), pow(pointNDotH,2)),2);
	
	float pointUl= dot(normal, pointLightDir);
	float pointUv = dot(normal, view);
	float pointG = 0.5 / mix(2 * pointUl * pointUv, pointUl + pointUv, pow(m,2));

	vec3 pointHighlight = pointD * pointG * pointFresnel;
	
	vec3 specularHighlight = directionalHighlight ;//* pointHighlight;
	vec3 specular = specularHighlight + specularReflection;
	outColor = vec4(pow(specular,vec3(1.0/2.2)), min(0.9, 1.0 - alpha));
}
