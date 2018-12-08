#pragma once

#include <glm.hpp>
#include "PagMaterial.h"
#include "PagCamera.h"

/*
* @brief	Clase Abstracta Pag3DElement qye representa una interfaz de objetos 3D
* @file		Pag3DElement.h 
* @author	Javier Serrano Casas
* @date		16-11-2018
*/
class Pag3DElement{
public:
	Pag3DElement();
	~Pag3DElement();

	virtual void drawAsTriangles(PagShaderProgram *shader, glm::mat4 vp, glm::mat4 v) = 0;
	virtual void drawAsPoints(PagShaderProgram *shader, glm::mat4 viewProject) = 0;
	virtual void drawAsLines(PagShaderProgram *shader, glm::mat4 viewProject) = 0;
	virtual void drawAsDebug(PagShaderProgram *shader, glm::mat4 viewProject) = 0;

	PagMaterial getMaterial();
	glm::mat4x4 getModelMatrix();
	void setMaterial(PagMaterial *_material);
	void setModelMatrix(const glm::mat4x4& _matrixM);

	// Métodos de transformación en la escena
	void translateModel(const glm::vec3& translation);
	void rotateModel(const glm::vec3& rotation, float angle);
	void scaleModel(const glm::vec3& scale);

private:
	glm::mat4 modelMatrix;	// Matriz de modelado del elemento
	PagMaterial material;	// Material del elemento
};

