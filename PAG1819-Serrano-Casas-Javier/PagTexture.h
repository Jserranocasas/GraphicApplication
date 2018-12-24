#pragma once

#include <glm.hpp>
#include "PagShaderProgram.h"

class PagTexture{
public:
	PagTexture(const char* filename, GLfloat texturesMode, GLfloat wrap_s, GLfloat wrap_t);
	PagTexture(const PagTexture &orig);
	PagTexture& operator=(const PagTexture &orig);
	~PagTexture();

	GLuint getTexture();
private:
	GLuint texture;
};