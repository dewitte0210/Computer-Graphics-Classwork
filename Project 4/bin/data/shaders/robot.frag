#version 410

uniform vec3 meshColor;
uniform vec3 ambientColor;
uniform vec3 dirLightColor;
uniform vec3 dirLightDir;
uniform vec3 spotLightColor;
uniform vec3 spotLightConeDir;
uniform vec3 spotLightPos;
uniform float spotLightCutoff;
uniform vec3 pointLightPos;
uniform vec3 pointLightColor;

in vec3 fragNormal;
in vec3 worldPosition;
out vec4 outColor;

void main(){
    //Directional Light Calculations
    float dirNDotL = max(0, dot(fragNormal, dirLightDir));
    vec3 directionalLight = dirLightColor * dirNDotL;

    //Point Light Calculations
    vec3 pointLightDir = pointLightPos - worldPosition;
    float pointNDotL = max(0, dot(fragNormal, pointLightDir));
    vec3 pointLight = pointLightColor * pointNDotL;
    
    //Spot Light Calculations
    vec3 toSpotLight = (spotLightPos - worldPosition);
    vec3 spotLightDir = normalize(toSpotLight);
    float cosAngle = step(spotLightCutoff, dot(spotLightDir, -spotLightConeDir));
    vec3 spotLight = spotLightColor * cosAngle;

    vec3 irradiance = directionalLight + pointLight + spotLight + ambientColor; 

    outColor = vec4(pow(meshColor * irradiance, vec3(1.0/2.0)),1.0);
}