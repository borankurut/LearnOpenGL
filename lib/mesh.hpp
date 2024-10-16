#pragma once

#include <glm/glm.hpp>
#include "shader.hpp"


struct Vertex{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh{

public:
	// mesh data
	std::vector<Vertex>			m_vertices;
	std::vector<unsigned int>	m_indices;
	std::vector<Texture>		m_textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void draw(Shader &shader);

private:
	// render data
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

