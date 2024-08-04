#pragma once

class ImageLoader{

public:
	static ImageLoader& get();

	unsigned char* loadImage(const char *filename, int* width, int* height, int* nrChannels, int req_comp) const;

	void freeImage(unsigned char * data) const;

private:
    ImageLoader();				// Constructor is private
    ~ImageLoader() = default;	// Destructor is private
							  
    ImageLoader(const ImageLoader&) = delete;
    ImageLoader& operator=(const ImageLoader&) = delete;
};
