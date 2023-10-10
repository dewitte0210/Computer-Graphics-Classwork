#version 410

uniform vec3 meshColor;
uniform vec3 camearPos;

in vec3 fragNormal;
in vec3 fragWorldPos;
out vec4 outColor;

void main(){
	vec3 normal = normalize(fragNormal);
	vec3 toCam = normalize(camearPos - fragWorldPos);
	
	float rimAmt = 1.0-max(0.0,dot(normal,toCam));
	rimAmt = pow(rimAmt, 2);

	vec3 rimLightColor = vec3(1,1,1);
	outColor = vec4(rimLightColor * rimAmt, 1.0);
}
