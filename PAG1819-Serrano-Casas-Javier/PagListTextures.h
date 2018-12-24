#pragma once

#include "PagTexture.h"
#include <vector>

class PagListTextures {
public:
	~PagListTextures();

	// - Método para acceder al singleton. 
	static PagListTextures *getInstance();

	// - Este método prepara las texturas
	void insertTexture(PagTexture *_texture);

	GLuint getTexture(int i);
private:
	// - Constructor es privado
	PagListTextures();

	// - Singleton. Única instancia que se tiene en la aplicación.
	static PagListTextures *instance;

	// - List de texturas.
	std::vector<PagTexture*> textures;
};