#include "image_loader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ImageLoader::ImageLoader(){
	stbi_set_flip_vertically_on_load(true);
}

ImageLoader& ImageLoader::get(){
	static ImageLoader instance;
	return instance;
}

ImageLoader::ImageInfo ImageLoader::loadImage(const char *filename, int req_comp) const{
	ImageLoader::ImageInfo imageInfo;

	imageInfo.data = stbi_load(filename, &imageInfo.width, &imageInfo.height, &imageInfo.nrChannels, req_comp);
	imageInfo.aspectRatio = (float) imageInfo.width / (float) imageInfo.height;

	return imageInfo;
}

void ImageLoader::freeImage(unsigned char * data) const{
	stbi_image_free(data);
}

void ImageLoader::freeImage(const ImageInfo& imageInfo) const{
	stbi_image_free(imageInfo.data);
}

