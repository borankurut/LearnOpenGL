#pragma once

#include <glm/glm.hpp>
#include <iostream>

inline void printVec3(glm::vec3 vec){
	std::cout << "x:" << vec.x << ", y:" << vec.y << ", z:" << vec.z;
}

inline void printVec4(glm::vec4 vec){
	std::cout << "x:" << vec.x << ", y:" << vec.y << ", z:" << vec.z << ", w:" << vec.w;
}

