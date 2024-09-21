#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

using Color = glm::vec3;

struct Light{

	Light(){}
	Light(glm::vec4 position_or_direction, Color ambient, Color diffuse, Color specular): 
		m_position_or_direction(position_or_direction), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular){/*empty*/}

	glm::vec4 m_position_or_direction;

	Color m_ambient;
	Color m_diffuse;
	Color m_specular;

	inline glm::vec3 position(){

		if(!isDirectional())
			return glm::vec3(m_position_or_direction);

		else
			std::cerr << "ERROR: Trying to get a position from Directional Light.\n";

		return glm::vec3(0.0f, 0.0f, 0.0f);
	}

	inline bool isDirectional(){return m_position_or_direction.w == 0.0f;}
};

struct PointLight : public Light{

	PointLight(){}
	PointLight(glm::vec4 position_or_direction, Color ambient, Color diffuse, Color specular, float constant, float linear, float quadratic):
		Light(position_or_direction, ambient, diffuse, specular), m_constant(constant), m_linear(linear), m_quadratic(quadratic){/*empty*/}

	float m_constant;
	float m_linear;
	float m_quadratic;

	static const PointLight defaultPointLight;
};


struct SpotLight : public PointLight{

	SpotLight(){}
	SpotLight(glm::vec4 position_or_direction, Color ambient, Color diffuse, Color specular, float constant, float linear, float quadratic, 
			glm::vec3 direction, float cutOff, float outerCutOff):
		PointLight(position_or_direction, ambient, diffuse, specular, constant, linear, quadratic), 
		m_direction(direction), m_cutOff(cutOff), m_outerCutOff(outerCutOff){/*empty*/}

	glm::vec3 m_direction;
	float m_cutOff;
	float m_outerCutOff;
};


