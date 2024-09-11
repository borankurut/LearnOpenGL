#version 330 core

out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emmisionMap;

	float shininess;
};

struct Light{
	vec4 position_or_direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;

in Light LightView;
in vec3 Normal;
in vec3 ViewFragPos;
in vec2 TexCoords;

void main(){
	//ambient
	vec3 ambient = LightView.ambient * vec3(texture(material.diffuse, TexCoords));

	vec3 lightDir = normalize(LightView.position_or_direction.xyz - ViewFragPos);


	//calculate the attenuation:
	float distance = length(LightView.position_or_direction.xyz - ViewFragPos); // TODO view mode length valid or not?
	float attenuation = 1.0 / (LightView.constant + LightView.linear * distance + LightView.quadratic * distance * distance);

	//diffuse
	vec3 norm = normalize(Normal);

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * LightView.diffuse * vec3(texture(material.diffuse, TexCoords));

	//specular
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(-ViewFragPos);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = spec * LightView.specular * vec3(texture(material.specular, TexCoords));

	// TODO: disable emmision if emmision map is not set. research this.
	//emmision
	// vec3 emmision = vec3(texture(material.emmisionMap, TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// vec3 result = ambient + diffuse + specular + emmision;
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}

