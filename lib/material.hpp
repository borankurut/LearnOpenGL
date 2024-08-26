#pragma once

#include <glm/vec3.hpp>

using Color = glm::vec3;

struct Material{
	Color ambient;
	Color diffuse;
	Color specular;

	float shininess;
};

