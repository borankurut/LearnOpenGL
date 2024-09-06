#include "texture.hpp"
#include <iostream>

Texture::Texture(const std::string& path, GLenum unit, GLenum internalFormat, GLenum format)
	: m_unit(unit), m_internalFormat(internalFormat), m_format(format){

	glGenTextures(1, &m_ID);
	glActiveTexture(m_unit);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	p_imageInfo = ImageLoader::get().loadImage(path.c_str(), 0);
	if(p_imageInfo.data){
		glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, p_imageInfo.width, p_imageInfo.height, 0, m_format, GL_UNSIGNED_BYTE, p_imageInfo.data);
		ImageLoader::get().freeImage(p_imageInfo);
	}
	else{
		std::cerr << "Couldn't load texture.\n";
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture(){
	glDeleteTextures(1, &m_ID);	

}

void Texture::use(const std::string& uniformName, const Shader & shader) const{
	glActiveTexture(m_unit);
	glBindTexture(GL_TEXTURE_2D, m_ID);
	shader.setInt(uniformName, m_unit - GL_TEXTURE0);
}

void Texture::unbind() const{
	glBindTexture(GL_TEXTURE_2D, 0);
}

