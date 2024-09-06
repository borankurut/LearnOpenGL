#pragma once

#include "image_loader.hpp"
#include "shader.hpp"
#include <glad/glad.h>
#include <string>

class Texture{

public:
	Texture(const std::string& path, GLenum unit, GLenum internalFormat, GLenum format);

	~Texture();

	void use(const std::string& uniformName, const Shader & shader) const;

	void unbind() const;

	ImageLoader::ImageInfo p_imageInfo;

private:
	unsigned int m_ID;
	GLenum m_unit;
	GLenum m_internalFormat;
	GLenum m_format;
};

