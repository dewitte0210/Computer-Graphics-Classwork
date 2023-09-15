#version 410

uniform sampler2D mandrilTex;
uniform sampler2D berriesTex;

in vec2 fragUV;
out vec4 outCol;

void main()
{
	vec4 mandril = texture(mandrilTex, fragUV);
	vec4 berries = texture(berriesTex, fragUV);
	outCol = mix(berries, mandril, 0.5);
}
