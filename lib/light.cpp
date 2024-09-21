#include "light.hpp"

const PointLight PointLight::defaultPointLight = PointLight(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec3(0.2f, 0.2f, 0.2f), 
			glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), .0f, 0.09f, 0.032f);


