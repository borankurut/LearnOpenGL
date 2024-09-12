#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

struct Light{
	vec4 position_or_direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	vec3 direction;
	float cutOff;
};

uniform Light light;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMatrix;

out vec3 Normal;
out vec3 ViewFragPos;
out Light LightView;
out vec2 TexCoords;

void main(){
	TexCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ViewFragPos = vec3(view * model * vec4(aPos, 1.0));
	Normal = normalMatrix * aNormal;

	// TODO this part still sucks.
	LightView.ambient = light.ambient;
	LightView.diffuse = light.diffuse;
	LightView.specular = light.specular;

	LightView.constant = light.constant;
	LightView.linear = light.linear;
	LightView.quadratic = light.quadratic;

	LightView.direction = light.direction;
	LightView.direction = light.direction;

    vec4 lightPositionView = view * light.position_or_direction; 
    LightView.position_or_direction = lightPositionView.xyzw;
};

