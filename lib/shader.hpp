#pragma once
#include <string>
#include <glm/matrix.hpp>
#include <light.hpp>
#include <material.hpp>

class Shader{

public:
	// program id
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use() const; // not const on the book so wtf?
					  
	void setBool(const std::string& name, bool value) const;
	void setFloat(const std::string& name, float value) const;
	void setInt(const std::string& name, int value) const;


	void setMat3(const std::string& name, glm::mat3 matrix) const;
	void setMat4(const std::string& name, glm::mat4 matrix) const;
	void setVec3(const std::string& name, glm::vec3 vec) const;
	void setLight(const std::string& name, const Light& light) const;
	void setMaterial(const std::string& name, const Material& material) const;

private:
	/*empty*/
};

