#version 410

layout (location = 0) in vec3 pos;

uniform mat4 mvp;

out vec3 fragPos;

void main()
{
	fragPos = pos;
	
	//Hack to put the skybox at the far plane
	gl_Position = (mvp * vec4(pos, 1.0)).xyww;
}