#pragma once
#include <string>

class Shader{

public:
	// program id
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use() const; // not const on the book so wtf?
					  
	void setBool(const std::string& name, bool value) const;
	void setFloat(const std::string& name, float value) const;
	void setInt(const std::string& name, int value) const;

private:
	/*empty*/
};

