#pragma once

#include "PagTexture.h"
#include <vector>

class PagListTextures {
public:
	~PagListTextures();

	// - M�todo para acceder al singleton. 
	static PagListTextures *getInstance();

	// - Este m�todo prepara las texturas
	void insertTexture(PagTexture *_texture);

	GLuint getTexture(int i);
private:
	// - Constructor es privado
	PagListTextures();

	// - Singleton. �nica instancia que se tiene en la aplicaci�n.
	static PagListTextures *instance;

	// - List de texturas.
	std::vector<PagTexture*> textures;
};