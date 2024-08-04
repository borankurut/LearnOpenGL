#include "image_loader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ImageLoader& ImageLoader::get(){
	static ImageLoader instance;
	return instance;
}

unsigned char* ImageLoader::loadImage(const char *filename, int* width, int* height, int* nrChannels, int req_comp) const{
	return stbi_load(filename, width, height, nrChannels, req_comp);
}

void ImageLoader::freeImage(unsigned char * data) const{
	stbi_image_free(data);
}

ImageLoader::ImageLoader(){
	stbi_set_flip_vertically_on_load(true);
}

