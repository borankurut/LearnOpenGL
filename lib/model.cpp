#include "model.hpp"
#include <iostream>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Model::Model(char* path){
	loadModel(path);
}

void Model::Draw(Shader &shader){
	for(unsigned int i = 0; i < m_meshes.size(); ++i){
		m_meshes[i].draw(shader);
	}
}

void Model::loadModel(std::string path){
	Assimp::Importer importer;

	// other useful: aiProcess_GenNormals, aiProcess_SplitLargeMeshes, aiProcess_OptimizeMeshes.
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	m_directory = path.substr(0, path.find_last_of("/")); // os dependent or not?

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene){
	for(unsigned int i = 0; i < node->mNumMeshes; ++i){
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}
	for(unsigned int i = 0; i < node->mNumChildren; ++i){
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene){

}


