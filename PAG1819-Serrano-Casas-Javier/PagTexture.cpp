#include "PagTexture.h"
#include "lodepng.h"
#include <vector>

PagTexture::PagTexture(const char* filename, GLfloat texturesMode, GLfloat wrap_s, GLfloat wrap_t) {
	texture = 0;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	/** Carga un png de disco https://lodev.org/lodepng/ */
	std::vector<unsigned char> image; // Los píxeles de la imagen
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);

	if (error) {
		std::cout << filename << " cannot be loaded" << std::endl;
		return;
	}

	// La textura se carga del revés, así que vamos a darle la vuelta
	unsigned char *imgPtr = &image[0];
	int numColorComponents = 4;
	int wInc = width * 4; //width in char
	unsigned char* top = nullptr;
	unsigned char* bot = nullptr;
	unsigned char temp = 0;

	for (int i = 0; i < height / 2; i++){
		top = imgPtr + i * wInc;
		bot = imgPtr + (height - i - 1) * wInc;
		for (int j = 0; j < wInc; j++){
			temp = *top;
			*top = *bot;
			*bot = temp;
			++top;
			++bot;
		}
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texturesMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texturesMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
	glGenerateMipmap(GL_TEXTURE_2D);
}


PagTexture::PagTexture(const PagTexture &orig) {
	texture = orig.texture;
}

PagTexture& PagTexture::operator=(const PagTexture &orig) {
	if (this != &orig) {
		texture = orig.texture;
	}
	return *this;
}

PagTexture::~PagTexture() {
}

GLuint PagTexture::getTexture() {
	return texture;
}
