#include "Pag3DGroup.h"
#include <gtc\matrix_transform.hpp>

Pag3DGroup::Pag3DGroup() : Pag3DElement() {
}

Pag3DGroup& Pag3DGroup::operator=(const Pag3DGroup& orig) {
	if (this != &orig) {
		elements = orig.elements;
	}
	return *this;
}

Pag3DGroup::~Pag3DGroup() {
}

void Pag3DGroup::insert3DElement(PagPlane *element) {
	elements.push_back(element);
}

void Pag3DGroup::insert3DElement(Pag3DGroup *element) {
	elements.push_back(element);
}

void Pag3DGroup::insert3DElement(PagRevolutionObject *element) {
	elements.push_back(element);
}

void Pag3DGroup::drawAsTriangles(PagShaderProgram *shader, glm::mat4 vp, glm::mat4 v) {
	glm::mat4 modelViewProject;
	glm::mat4 modelView;

	for (unsigned int i = 0; i < elements.size(); i++) {
		modelViewProject = vp * elements[i]->getModelMatrix();
		modelView = v * elements[i]->getModelMatrix();

		// - Asignamos la matriz de vision, modelado y proyeccion de tipo uniform del shader program.
		shader->setUniform("mModelViewProj", modelViewProject);
		shader->setUniform("mModelView", modelView);

		elements[i]->getMaterial().sendUniform(shader);
		elements[i]->drawAsTriangles(shader, modelViewProject, modelView);
	}
}

void Pag3DGroup::drawAsPoints(PagShaderProgram *shader, glm::mat4 viewProject) {
	glm::mat4 modelViewProject;

	for (unsigned int i = 0; i < elements.size(); i++) {
		modelViewProject = viewProject * elements[i]->getModelMatrix();

		// - Asignamos la matriz de vision, modelado y proyeccion de tipo uniform del shader program.
		shader->setUniform("mModelViewProj", modelViewProject);

		elements[i]->drawAsPoints(shader, modelViewProject);
	}
}

// - 
void Pag3DGroup::drawAsLines(PagShaderProgram *shader, glm::mat4 viewProject) {
	glm::mat4 modelViewProject;

	for (unsigned int i = 0; i < elements.size(); i++) {
		modelViewProject = viewProject * elements[i]->getModelMatrix();

		// - Asignamos la matriz de vision, modelado y proyeccion de tipo uniform del shader program.
		shader->setUniform("mModelViewProj", modelViewProject);

		elements[i]->drawAsLines(shader, modelViewProject);
	}
}
