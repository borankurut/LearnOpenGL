#version 330 core

out vec4 FragColor;

struct Material{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;

	float shininess;

};

struct DirLight{
	vec4 position_or_direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight{
	vec4 position_or_direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight{
	vec4 position_or_direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	vec3 direction;
	float cutOff;
	float outerCutOff;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform DirLight dirLight;

#define PointLightAmount 5
uniform PointLight pointLights[PointLightAmount];
uniform SpotLight spotLight;

uniform vec3 viewPosition;


vec3 CalculateDirectionalLight(DirLight dirLight, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir);
vec3 FixMinusVec3(vec3 vec);
vec3 ShadeLight();
vec3 ShadeLightFixNullLights();

void main(){

	// vec3 result = ShadeLight();
	vec3 result = ShadeLightFixNullLights();

	FragColor = vec4(result, 1.0);
}

vec3 CalculateDirectionalLight(DirLight dirLight, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-dirLight.position_or_direction.xyz);

	// ambient
	vec3 ambient = dirLight.ambient * vec3(texture(material.diffuse, TexCoords));

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * dirLight.diffuse * vec3(texture(material.diffuse, TexCoords));

	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * dirLight.specular * vec3(texture(material.specular, TexCoords));

	return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(pointLight.position_or_direction.xyz - FragPos);

	// ambient
	vec3 ambient = pointLight.ambient * vec3(texture(material.diffuse, TexCoords));

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * pointLight.diffuse * vec3(texture(material.diffuse, TexCoords));

	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * pointLight.specular * vec3(texture(material.specular, TexCoords));

	// attenuation
	float distance = length(pointLight.position_or_direction.xyz - FragPos);
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance); 

	return ambient * attenuation + diffuse * attenuation + specular * attenuation;
} // book gives fragpos as argument, why?

vec3 CalculateSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(spotLight.position_or_direction.xyz - FragPos);

	// ambient
	vec3 ambient = spotLight.ambient * vec3(texture(material.diffuse, TexCoords));

	// diffuse
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * spotLight.diffuse * vec3(texture(material.diffuse, TexCoords));

	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * spotLight.specular * vec3(texture(material.specular, TexCoords));

	// attenuation
	float distance = length(spotLight.position_or_direction.xyz - FragPos);
	float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * distance * distance); 

	// intensity
	float theta = dot(lightDir, normalize(-spotLight.direction));
	float epsilon = spotLight.cutOff - spotLight.outerCutOff;
	float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

	// return diffuse; 
	return ambient * attenuation + diffuse * attenuation * intensity + specular * attenuation * intensity;
}

vec3 FixMinusVec3(vec3 vec){
	return vec3(max(vec.x, 0.0), max(vec.y, 0.0), max(vec.z, 0.0));
}

vec3 ShadeLight(){
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPosition - FragPos);

	vec3 result = CalculateDirectionalLight(dirLight, normal, viewDir);

	for(int i = 0; i < PointLightAmount; ++i)
		result += CalculatePointLight(pointLights[i], normal, viewDir);

	result += CalculateSpotLight(spotLight, normal, viewDir);

	return result;
}

vec3 ShadeLightFixNullLights(){
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPosition - FragPos);

	vec3 result = FixMinusVec3(CalculateDirectionalLight(dirLight, normal, viewDir));

	for(int i = 0; i < PointLightAmount; ++i)
		result += FixMinusVec3(CalculatePointLight(pointLights[i], normal, viewDir));

	result += FixMinusVec3(CalculateSpotLight(spotLight, normal, viewDir));

	return result;
}
