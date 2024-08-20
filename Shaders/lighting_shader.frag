#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 Normal;
in vec3 ActualFragPos;

void main() {
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize(lightPos - ActualFragPos); //towards light
	float diffuseStrength = max(dot(lightDir, Normal), 0.0);
	vec3 diffuse = diffuseStrength * lightColor;

	vec3 result = objectColor * (ambient + diffuse);



	FragColor = vec4(result, 1.0);
}

