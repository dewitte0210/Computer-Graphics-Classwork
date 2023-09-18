#version 410

in vec2 fragUV;
uniform sampler2D tex;
out vec4 outCol;

void main(){
	outCol = texture(tex, fragUV);
	outCol.a = min(outCol.a, 0.8);
}