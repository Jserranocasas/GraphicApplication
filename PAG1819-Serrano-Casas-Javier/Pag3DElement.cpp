#include "Pag3DElement.h"
#include <gtc\matrix_transform.hpp>

Pag3DElement::Pag3DElement() : material() {
	modelMatrix = glm::mat4(1.0f);
}

Pag3DElement::~Pag3DElement() {
}

PagMaterial Pag3DElement::getMaterial() {
	return material;
}

void Pag3DElement::setMaterial(glm::vec3 _kd, glm::vec3 _ks, float _shi) {
	material.setMaterial(_kd, _ks, _shi);
}

glm::mat4x4 Pag3DElement::getModelMatrix() {
	return modelMatrix;
}

void Pag3DElement::setModelMatrix(const glm::mat4x4& _matrixM) {
	modelMatrix = _matrixM;
}

void Pag3DElement::translateModel(const glm::vec3& translation) {
	modelMatrix = glm::translate(modelMatrix, translation);
}

void Pag3DElement::rotateModel(const glm::vec3& rotation, float angle) {
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), rotation);
}

void Pag3DElement::scaleModel(const glm::vec3& scale) {
	modelMatrix = glm::scale(modelMatrix, scale);
}