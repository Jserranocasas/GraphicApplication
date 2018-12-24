#include "Pag3DElement.h"
#include <gtc\matrix_transform.hpp>

Pag3DElement::Pag3DElement() : material() {
	modelMatrix = glm::mat4(1.0f);
}

Pag3DElement::~Pag3DElement() {
}

PagMaterial& Pag3DElement::getMaterial() {
	return material;
}

void Pag3DElement::setMaterial(PagMaterial* _material) {
	material = *_material;
}

glm::mat4x4 Pag3DElement::getModelMatrix() {
	return modelMatrix;
}

void Pag3DElement::setModelMatrix(const glm::mat4x4& _matrixM) {
	modelMatrix = _matrixM;
}

// - Traslada el objeto segun el vector pasado por parámetro
void Pag3DElement::translateModel(const glm::vec3& translation) {
	modelMatrix = glm::translate(modelMatrix, translation);
}

// - Rota el objeto segun un angulo y el eje sobre el que rotará
void Pag3DElement::rotateModel(const glm::vec3& rotation, float angle) {
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), rotation);
}

// - Escala el objeto segun el vector pasado por parámetro
void Pag3DElement::scaleModel(const glm::vec3& scale) {
	modelMatrix = glm::scale(modelMatrix, scale);
}