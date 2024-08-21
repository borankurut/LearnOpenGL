#version 330 core

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

vec3 ViewPos = vec3(0.0, 0.0, 0.0);

in vec3 Normal;
in vec3 ViewFragPos;

void main() {

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

	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = objectColor * (ambient + diffuse + specular);

	FragColor = vec4(result, 1.0);
}

