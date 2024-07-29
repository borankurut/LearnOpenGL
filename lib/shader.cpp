#include "shader.hpp"
#include "glad/glad.h"
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
		std::cout << vertexPath << std::endl;
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

