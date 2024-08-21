#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMatrix;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

vec3 Normal;
vec3 ViewFragPos;

vec3 ViewPos = vec3(0.0, 0.0, 0.0);

out vec4 Color;

void main(){
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ViewFragPos = vec3(view * model * vec4(aPos, 1.0));
	Normal = normalMatrix * aNormal;

	//ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	//diffuse
	vec3 lightDir = normalize(lightPos - ViewFragPos); //towards light
	vec3 norm = normalize(Normal);

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;

	//specular
	float specularStrength = 0.5f;
	vec3 reflectDir = reflect(-lightDir, norm); // -lightDir is towards the fragment.
												// apperantly it returns normalized if its inputs are normalized, idk math.
												
	vec3 viewDir = normalize(-ViewFragPos);			// from fragment to camera, since camera is on (0, 0, 0)

	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 8);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = objectColor * (ambient + diffuse + specular);

	Color = vec4(result, 1.0);

};

