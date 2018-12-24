#include "PagMaterial.h"

PagMaterial::PagMaterial() : kd(glm::vec3(1.0, 1.0, 1.0)), ks(glm::vec3(1.0, 1.0, 1.0)), shininess(1.0) {
	textures = new std::vector<int>;
}

PagMaterial::PagMaterial(glm::vec3 _kd, glm::vec3 _ks, float shi) : kd(_kd), ks(_ks) {
	if (shi < 1.0) {
		shininess = 1.0;
	}
	textures = new std::vector<int>;
	shininess = shi;
}

PagMaterial::PagMaterial(const PagMaterial &orig) {
	shininess = orig.shininess;
	textures = orig.textures;
	kd = orig.kd;
	ks = orig.ks;
}

PagMaterial& PagMaterial::operator=(const PagMaterial &orig) {
	if (this != &orig) {
		shininess = orig.shininess;
		textures = orig.textures;
		kd = orig.kd;
		ks = orig.ks;
	}
	return *this;
}

PagMaterial::~PagMaterial() {
}

void PagMaterial::setMaterial(glm::vec3 _kd, glm::vec3 _ks, float _shi) {
	shininess = _shi;
	kd = _kd;
	ks = _ks;
}

glm::vec3 PagMaterial::getKd() {
	return kd;
}

glm::vec3 PagMaterial::getKs() {
	return ks;
}

float PagMaterial::getShininess() {
	return shininess;
}

void PagMaterial::addTexture(int texture) {
	textures->push_back(texture);
}

int PagMaterial::getTexture(int i) {
	return textures->at(i);
}

int PagMaterial::numTextures() {
	return textures->size();
}