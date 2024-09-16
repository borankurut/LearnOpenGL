#include "shader.hpp"
#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

#define INFO_LOG_BF_SIZE 512

Shader::Shader(const char* vertexPath, const char* fragmentPath){
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	vertexFile.exceptions(std::fstream::failbit | std::fstream::badbit);
	fragmentFile.exceptions(std::fstream::failbit | std::fstream::badbit);

	try{
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);

		std::stringstream vertexStream;
		std::stringstream fragmentStream;

		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();

		vertexFile.close();
		fragmentFile.close();

		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();

	}
	catch(std::ifstream::failure e){
		std::cerr << "ERROR READING SHADER FILE:" << e.what() << std::endl;
	}

	const char* vertexCodeCstr = vertexCode.c_str();
	const char* fragmentCodeCstr = fragmentCode.c_str();

	unsigned int vertex;
	unsigned int fragment;

	int success;
	char infoLog[INFO_LOG_BF_SIZE];

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCodeCstr, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(vertex, INFO_LOG_BF_SIZE, NULL, infoLog);
		std::cerr << "ERROR COMPILING VERTEX SHADER: " << infoLog << std::endl;
	}

	//fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCodeCstr, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if(!success){
		glGetShaderInfoLog(fragment, INFO_LOG_BF_SIZE, NULL, infoLog);
		std::cerr << "ERROR COMPILING FRAGMENT SHADER: " << infoLog << std::endl;
	}

	// shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(ID, INFO_LOG_BF_SIZE, NULL, infoLog);
		std::cerr << "ERROR LINKING SHADERS: " << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() const{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);

}

void Shader::setInt(const std::string& name, int value) const{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat3(const std::string& name, glm::mat3 matrix) const{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::setMat4(const std::string& name, glm::mat4 matrix) const{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const std::string& name, glm::vec3 vec) const{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setVec4(const std::string& name, glm::vec4 vec) const{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setLight(const std::string& name, const Light& light) const{
	setVec3(name + ".ambient", light.ambient);
	setVec3(name + ".diffuse", light.diffuse);
	setVec3(name + ".specular", light.specular);
	setVec4(name + ".position_or_direction", light.position_or_direction);
}

void Shader::setDirLight(const std::string& name, const Light& light) const{
	if(light.position_or_direction.w != 0.0f){
		std::cerr << "Trying to set a directional light that its w is not 0.\n";
		return;
	}

	setVec3(name + ".ambient", light.ambient);
	setVec3(name + ".diffuse", light.diffuse);
	setVec3(name + ".specular", light.specular);
	setVec4(name + ".position_or_direction", light.position_or_direction);
}

void Shader::setPointLight(const std::string& name, const PointLight& light) const{
	if(light.position_or_direction.w == 0.0f){
		std::cerr << "Trying to set a point light that its w is 0.\n";
		return;
	}

	setVec3(name + ".ambient", light.ambient);
	setVec3(name + ".diffuse", light.diffuse);
	setVec3(name + ".specular", light.specular);
	setVec4(name + ".position_or_direction", light.position_or_direction);
	setFloat(name + ".constant", light.constant);
	setFloat(name + ".linear", light.linear);
	setFloat(name + ".quadratic", light.quadratic);
}

void Shader::setPointLightInArray(const std::string& arrayName, int index, const PointLight& light) const{
	std::string toSetName = arrayName + "[" + std::to_string(index) + "]";
	setPointLight(toSetName, light);
}

void Shader::setPointLights(const std::string& arrayName, std::vector<PointLight> lights) const{
	for(int i = 0; i < lights.size(); ++i){
		setPointLightInArray(arrayName, i, lights[i]);
	}
}

void Shader::setSpotLight(const std::string& name, const SpotLight& light) const{
	setVec3(name + ".ambient", light.ambient);
	setVec3(name + ".diffuse", light.diffuse);
	setVec3(name + ".specular", light.specular);
	setVec4(name + ".position_or_direction", light.position_or_direction);
	setFloat(name + ".constant", light.constant);
	setFloat(name + ".linear", light.linear);
	setFloat(name + ".quadratic", light.quadratic);
	setVec3(name + ".direction", light.direction);
	setFloat(name + ".cutOff", light.cutOff);
	setFloat(name + ".outerCutOff", light.outerCutOff);
}

void Shader::setMaterial(const std::string& name, const Material& material) const{
	setVec3(name + ".ambient", material.ambient);
	setVec3(name + ".diffuse", material.diffuse);
	setVec3(name + ".specular", material.specular);
	setFloat(name + ".shininess", material.shininess);
}

