#version 330 core

/* This shader use world space coordinates, not view space. TODO: Research to implement this in view space.*/

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

	vec3 direction;
	float cutOff;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPosition;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main(){
	//ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	vec3 lightDir = normalize(light.position_or_direction.xyz - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));

	if (theta < light.cutOff){
		FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
		
		FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
		return;
	}

	//calculate the attenuation:
	float distance = length(light.position_or_direction.xyz - FragPos); // TODO view mode length valid or not?
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

	//diffuse
	vec3 norm = normalize(Normal);

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

	//specular
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(viewPosition - FragPos);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
	vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoords));

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

