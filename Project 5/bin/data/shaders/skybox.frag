#version 410

uniform samplerCube cubeMap;

in vec3 fragPos;

out vec4 outColor;

void main()
{
	outColor = texture(cubeMap, fragPos);
}
