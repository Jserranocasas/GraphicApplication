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

// - Devuelve el tipo de elemento
ElementType Pag3DGroup::getElementType() {
	return PAG_GROUP;
}

// - Inserta al vector un objeto Pag3DElement de tipo PagPlane
void Pag3DGroup::insert3DElement(PagPlane *element) {
	elements.push_back(element);
}

// - Inserta al vector un objeto Pag3DElement de tipo Pag3DGroup
void Pag3DGroup::insert3DElement(Pag3DGroup *element) {
	elements.push_back(element);
}

// - Inserta al vector un objeto Pag3DElement de tipo PagRevolutionObject
void Pag3DGroup::insert3DElement(PagRevolutionObject *element) {
	elements.push_back(element);
}

void Pag3DGroup::drawAsTextures(PagShaderProgram *shader, glm::mat4 vp, glm::mat4 v, std::vector<GLuint> texture){
	glm::mat4 modelViewProject;
	glm::mat4 modelView;

	for (unsigned int i = 0; i < elements.size(); i++) {
		modelViewProject = vp * elements[i]->getModelMatrix();
		modelView = v * elements[i]->getModelMatrix();

		// - Asignamos la matriz de vision, modelado y proyeccion de tipo uniform del shader program.
		shader->setUniform("mModelViewProj", modelViewProject);
		shader->setUniform("mModelView", modelView);
		shader->setUniform("Shininess", elements[i]->getMaterial().getShininess());
		shader->setUniform("Ks", elements[i]->getMaterial().getKs());

		if (elements[i]->getElementType() == PAG_GROUP) {
			elements[i]->drawAsTextures(shader, modelViewProject, modelView);
		}
		else {
			texture = std::vector<GLuint>();
			for( int j = 0; j < elements[i]->getMaterial().numTextures(); j++){
				GLuint idTexture = PagListTextures::getInstance()->getTexture(elements[i]->getMaterial().getTexture(0));
				texture.push_back(idTexture);
			}

			shader->setUniform("TexSamplerColor", 0);
			
			if (texture.size() == 2) {
				shader->setUniform("TexSamplerBump", 1);
			}

			elements[i]->drawAsTextures(shader, modelViewProject, modelView, texture);
		}
	}
}

// - Método para dibujar el grupo 3D de manera recursiva en modo triangulos sombreados
void Pag3DGroup::drawAsTriangles(PagShaderProgram *shader, glm::mat4 vp, glm::mat4 v) {
	glm::mat4 modelViewProject;
	glm::mat4 modelView;

	for (unsigned int i = 0; i < elements.size(); i++) {
		modelViewProject = vp * elements[i]->getModelMatrix();
		modelView = v * elements[i]->getModelMatrix();

		// - Asignamos la matriz de vision, modelado y proyeccion de tipo uniform del shader program.
		shader->setUniform("mModelViewProj", modelViewProject);
		shader->setUniform("mModelView", modelView);
		shader->setUniform("Shininess", elements[i]->getMaterial().getShininess());
		shader->setUniform("Kd", elements[i]->getMaterial().getKd());
		shader->setUniform("Ks", elements[i]->getMaterial().getKs());

		elements[i]->drawAsTriangles(shader, modelViewProject, modelView);
	}
}

// - Método para dibujar el grupo 3D de manera recursiva en modo nube de puntos
void Pag3DGroup::drawAsPoints(PagShaderProgram *shader, glm::mat4 viewProject) {
	glm::mat4 modelViewProject;

	for (unsigned int i = 0; i < elements.size(); i++) {
		modelViewProject = viewProject * elements[i]->getModelMatrix();

		// - Asignamos la matriz de vision, modelado y proyeccion de tipo uniform del shader program.
		shader->setUniform("mModelViewProj", modelViewProject);

		elements[i]->drawAsPoints(shader, modelViewProject);
	}
}

// - Método para dibujar el grupo 3D de manera recursiva en modo debug
void Pag3DGroup::drawAsDebug(PagShaderProgram *shader, glm::mat4 viewProject) {
	glm::mat4 modelViewProject;

	for (unsigned int i = 0; i < elements.size(); i++) {
		modelViewProject = viewProject * elements[i]->getModelMatrix();

		// - Asignamos la matriz de vision, modelado y proyeccion de tipo uniform del shader program.
		shader->setUniform("mModelViewProj", modelViewProject);

		elements[i]->drawAsDebug(shader, modelViewProject);
	}
}

// - Método para dibujar el grupo 3D de manera recursiva en modo lineas
void Pag3DGroup::drawAsLines(PagShaderProgram *shader, glm::mat4 viewProject) {
	glm::mat4 modelViewProject;

	for (unsigned int i = 0; i < elements.size(); i++) {
		modelViewProject = viewProject * elements[i]->getModelMatrix();

		// - Asignamos la matriz de vision, modelado y proyeccion de tipo uniform del shader program.
		shader->setUniform("mModelViewProj", modelViewProject);

		elements[i]->drawAsLines(shader, modelViewProject);
	}
}
