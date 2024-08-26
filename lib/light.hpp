#pragma once

#include <glm/vec3.hpp>

using Color = glm::vec3;

struct Light{
	glm::vec3 position;

	Color ambient;
	Color diffuse;
	Color specular;
};

