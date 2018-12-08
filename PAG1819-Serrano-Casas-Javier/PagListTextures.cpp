#include "PagListTextures.h"

// - Inicializa el singleton.
PagListTextures * PagListTextures::instance = nullptr;

// M�todos constructores -----------------------------------------
PagListTextures::PagListTextures() {}

// M�todos -------------------------------------------------------
PagListTextures::~PagListTextures() {
}

// - Acceder al singleton.
PagListTextures * PagListTextures::getInstance() {
	if (!instance) {
		instance = new PagListTextures();
	}
	return instance;
}

/* - M�todo que prepara los materiales */
void PagListTextures::insertTexture(PagTexture *_texture) {
	textures.push_back(*_texture);
}

PagTexture* PagListTextures::getTexture(int i) {
	return &textures[i];
}