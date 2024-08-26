#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMatrix;

out vec3 Normal;
out vec3 ViewFragPos;
out Light LightView;

void main(){
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ViewFragPos = vec3(view * model * vec4(aPos, 1.0));
	Normal = normalMatrix * aNormal;

	LightView.ambient = light.ambient;
	LightView.diffuse = light.diffuse;
	LightView.specular = light.specular;

    vec4 lightPositionView = view * vec4(light.position, 1.0);
    LightView.position = lightPositionView.xyz;
};

