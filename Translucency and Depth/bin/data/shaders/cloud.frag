#version 410

in vec2 fragUV;
uniform sampler2D tex;
out vec4 outCol;

void main(){
	outCol = texture(tex, fragUV);
	if(outCol.a > 0.8) outCol.a = 0.8;
}