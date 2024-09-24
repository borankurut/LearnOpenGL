#pragma once
#include "glad/glad.h"

// TODO: singleton is shit?
class ImageLoader{

public:
	struct ImageInfo{
		int width;
		int height;
		int nrChannels;
		float aspectRatio; // TODO: remove later
		float getAspectRatio(){return (float) width / (float) height;}
		unsigned char* data;
		GLenum format;
	};

	static ImageLoader& get();

	ImageInfo loadImage(const char *filename, int req_comp = 0) const;

	void freeImage(unsigned char * data) const;
	void freeImage(const ImageInfo& imageInfo) const;

private:
    ImageLoader();				// Constructor is private
    ~ImageLoader() = default;	// Destructor is private
							  
    ImageLoader(const ImageLoader&) = delete;
    ImageLoader& operator=(const ImageLoader&) = delete;
};

