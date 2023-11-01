#version 410

uniform vec3 meshColor;
uniform vec3 ambientColor;
uniform vec3 dirLightColor;
uniform vec3 dirLightDir;
uniform vec3 spotLightColor;
uniform vec3 spotLightConeDir;
uniform vec3 spotLightPos;
uniform vec3 spotLightCutoff;

in vec3 fragNormal;
in vec3 worldPosition;
out vec4 outColor;

void main(){
    //Directional Light Calculations
    float dirNDotL = max(0, dot(fragNormal, dirLightDir));

    vec3 irradiance = ambientColor + dirLightColor * dirNDotL; 

    outColor = vec4(pow(meshColor * irradiance, vec3(1.0/2.0)),1.0);
}