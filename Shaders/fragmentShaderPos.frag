#version 330 core

out vec4 FragColor;
in vec4 vertex_position;

void main() {
	FragColor = vertex_position; 
}

