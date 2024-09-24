#pragma once

#include "mesh.hpp"

#include <assimp/scene.h>

class Model{

public:
	Model(char* path);
	void Draw(Shader &shader);

private:
	// model data
	std::vector<Mesh> m_meshes;
	std::string m_directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* material, 
			aiTextureType textureType, std::string typeName);
	static unsigned int TextureFromFile(const char *path, const std::string &directory);
};


