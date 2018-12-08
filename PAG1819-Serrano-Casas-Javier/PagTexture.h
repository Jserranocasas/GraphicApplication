#pragma once

#include <glm.hpp>
#include "PagShaderProgram.h"

class PagTexture{
public:
	PagTexture(const char* filename);
	~PagTexture();

	void drawTexture(PagShaderProgram& shader, GLuint vaoObject, int indexesSize);
};