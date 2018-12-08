#pragma once

#include <vector>
#include "PagPlane.h"
#include "Pag3DElement.h"
#include "PagRevolutionObject.h"

/*
 * @brief	Clase que encapsula un conjunto de elementos Pag3DElement
 * @file	Pag3DGroup.h
 * @author	Javier Serrano Casas
 * @date	20-11-2018
 */
class Pag3DGroup : public Pag3DElement {
public:
	Pag3DGroup();
	~Pag3DGroup();

	Pag3DGroup& operator=(const Pag3DGroup& orig);
	
	// Métodos para insertar cualquier tipo de objeto 3D
	void insert3DElement(PagPlane *element);
	void insert3DElement(Pag3DGroup *element);
	void insert3DElement(PagRevolutionObject *element);

	// Modos de dibujar los objetos
	void drawAsTriangles(PagShaderProgram *shader, glm::mat4 vp, glm::mat4 v);
	void drawAsPoints(PagShaderProgram *shader, glm::mat4 viewProject);
	void drawAsLines(PagShaderProgram *shader, glm::mat4 viewProject);
	void drawAsDebug(PagShaderProgram *shader, glm::mat4 viewProject);

private:
	std::vector<Pag3DElement*> elements;  // Conjunto de elementos que forman el grupo 3D
};