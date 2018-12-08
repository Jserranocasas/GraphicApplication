#include "PagMaterial.h"

PagMaterial::PagMaterial() : kd(glm::vec3(1.0, 1.0, 1.0)), ks(glm::vec3(1.0, 1.0, 1.0)), shininess(0.0) {
}

PagMaterial::PagMaterial(glm::vec3 _kd, glm::vec3 _ks, float shi) : shininess(shi), kd(_kd), ks(_ks) {
}

PagMaterial::PagMaterial(const PagMaterial &orig) {
	shininess = orig.shininess;
	kd = orig.kd;
	ks = orig.ks;
}

PagMaterial& PagMaterial::operator=(const PagMaterial &orig) {
	if (this != &orig) {
		shininess = orig.shininess;
		kd = orig.kd;
		ks = orig.ks;
	}
	return *this;
}


PagMaterial::~PagMaterial() {
}

void PagMaterial::sendUniform(PagShaderProgram *shader) {
	shader->setUniform("Shininess", shininess);
	shader->setUniform("Kd", kd);
	shader->setUniform("Ks", ks);
}

void PagMaterial::setMaterial(glm::vec3 _kd, glm::vec3 _ks, float _shi) {
	shininess = _shi;
	kd = _kd;
	ks = _ks;
}
