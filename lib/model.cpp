#include "model.hpp"
#include <iostream>
#include <glad/glad.h>
#include "image_loader.hpp"

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
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for(unsigned int i = 0; i < mesh->mNumVertices; ++i){
		Vertex vertex;

		// process vertex;
		
		// position
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		
		// normal
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		
		// texcoords
		if(mesh->mTextureCoords[0]){ //mTextureCoords[0] is usually uv?
			glm::vec2 tcoords;

			tcoords.x = mesh->mTextureCoords[0][i].x;
			tcoords.y = mesh->mTextureCoords[0][i].y;

			vertex.TexCoords = tcoords;
		}
		else{
			vertex.TexCoords = glm::vec2(0.0, 0.0);
		}

		vertices.push_back(vertex);
	}

	// process indices;
	for(unsigned int i = 0; i < mesh->mNumFaces; ++i){
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; ++j){
			indices.push_back(face.mIndices[j]);
		}
	}
	
	// process material;
	if(mesh->mMaterialIndex >= 0){
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

	}
	return Mesh(vertices, indices, textures);
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material, 
		aiTextureType textureType, std::string typeName){

	std::vector<Texture> textures;
	for(unsigned int i = 0; i < material->GetTextureCount(textureType); ++i){
		aiString str;
		material->GetTexture(textureType, i, &str);

		Texture texture;
		texture.id = TextureFromFile(str.C_Str(), m_directory);
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
	}
	return textures;
}

unsigned int Model::TextureFromFile(const char *path, const std::string &directory)
{
	std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

	ImageLoader::ImageInfo img = ImageLoader::get().loadImage(filename.c_str());

    if (img.data)
    {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, img.format, img.width, img.height, 0, img.format, GL_UNSIGNED_BYTE, img.data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		ImageLoader::get().freeImage(img);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
		ImageLoader::get().freeImage(img);
    }

    return textureID;
}

