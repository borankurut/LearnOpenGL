#include "mesh.hpp"
#include "glad/glad.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures):
		m_vertices(vertices), m_indices(indices), m_textures(textures) {
			setupMesh();
		} 


void Mesh::setupMesh(){
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
	glEnableVertexAttribArray(0);

	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);

	// texcoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);

	// unbind
	glBindVertexArray(0);
}


void Mesh::draw(Shader &shader){
	unsigned int diffuseNr = 0;
	unsigned int specularNr = 0;

	for(unsigned int i = 0; i < m_textures.size(); ++i){
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = m_textures[i].type;
		if(name == "texture_diffuse"){
			number = std::to_string(diffuseNr++);

		}
		else if(name == "texture_specular"){
			number = std::to_string(specularNr++);
		}

		shader.setFloat(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


