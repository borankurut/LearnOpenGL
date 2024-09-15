#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>

using Color = glm::vec3;

struct Light{
	glm::vec4 position_or_direction;

	Color ambient;
	Color diffuse;
	Color specular;

	inline glm::vec3 position(){

		if(!isDirectional())
			return glm::vec3(position_or_direction);

		else
			std::cerr << "ERROR: Trying to get a position from Directional Light.\n";

		return glm::vec3(0.0f, 0.0f, 0.0f);
	}

	inline bool isDirectional(){return position_or_direction.w == 0.0f;}
};

struct PointLight : public Light{
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight : public PointLight{
	glm::vec3 direction;
	float cutOff;
	float outerCutOff;
};

