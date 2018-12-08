#include "PagTexture.h"
#include "lodepng.h"
#include <vector>

PagTexture::PagTexture(const char* filename) {
	/** Carga un png de disco https://lodev.org/lodepng/ */
	std::vector<unsigned char> image; // Los píxeles de la imagen
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, filename);
	if (error)
	{
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
	for (int i = 0; i < height / 2; i++)
	{
		top = imgPtr + i * wInc;
		bot = imgPtr + (height - i - 1) * wInc;
		for (int j = 0; j < wInc; j++)
		{
			temp = *top;
			*top = *bot;
			*bot = temp;
			++top;
			++bot;
		}
	}
}

PagTexture::~PagTexture() {
}

void PagTexture::drawTexture(PagShaderProgram& shader, GLuint vaoObject, int indexesSize) {
	shader.setUniform("TexSamplerColor", 0);

	//glBindVertexArray(vaoObject);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glDrawElements(GL_TRIANGLE_STRIP, indexesSize, GL_UNSIGNED_INT, NULL);

	//// Finalmente, pasamos la textura a OpenGL
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
}