#version 330 core

out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	sampler2D specular;

	float shininess;
};

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;

in Light LightView;
in vec3 Normal;
in vec3 ViewFragPos;
in vec2 TexCoords;

void main(){
	//ambient
	vec3 ambient = LightView.ambient * vec3(texture(material.diffuse, TexCoords));

	//diffuse
	vec3 lightDir = normalize(LightView.position - ViewFragPos);
	vec3 norm = normalize(Normal);

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * LightView.diffuse * vec3(texture(material.diffuse, TexCoords));

	//specular
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(-ViewFragPos);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = spec * LightView.specular * vec3(texture(material.specular, TexCoords));

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}

