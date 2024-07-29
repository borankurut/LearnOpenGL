#version 330 core

layout (location = 0) in vec3 aPos;
out vec4 vertex_position;

void main(){
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	vertex_position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
};

