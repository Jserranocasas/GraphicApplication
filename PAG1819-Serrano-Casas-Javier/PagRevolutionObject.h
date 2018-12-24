#pragma once

#include <vector>
#include <glm.hpp>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "PagVAO.h"
#include "PagCamera.h"
#include "Pag3DElement.h"
#include "PagStructures.h"
#include "PagEnumerations.h"
#include "PagSubdivisionProfile.h"

/*
* @file		PagRevolutionObject.h
* @brief	Clase que representa un objeto revolucionado a partir de un 
*			perfil que contendrá normales, posiciones, coordenadas de 
*			textura y tangentes del objeto revolucionado.
* @author	Javier Serrano Casas
* @date		16-10-2018
*/
class PagRevolutionObject : public Pag3DElement{
private:
	PagSubdivisionProfile profile;	//Perfil del objeto de revolución.

	PagVAO *bottom;		//Tapa inferior del objeto.
	PagVAO *body;		//Cuerpo del objeto.
	PagVAO *top;		//Tapa superior del objeto.

	// Métodos privados para la inicialización del objeto
	void computePosNormTex(PagRevObjParts part, unsigned int slices);
	void computeTangents(PagRevObjParts part, unsigned int slices);
	void defineWireFrame(PagRevObjParts part, unsigned int slices);
	void defineTopology(PagRevObjParts parts, unsigned int slices);
	void defineTMesh(PagRevObjParts part, unsigned int slices);
	void defineCloud(PagRevObjParts part, unsigned int slices);
public:
	PagRevolutionObject(std::vector<glm::vec2> points, 
		unsigned int subdivisions, unsigned int slices);
	PagRevolutionObject(const PagRevolutionObject &orig);
	PagRevolutionObject& operator=(const PagRevolutionObject &orig);
	~PagRevolutionObject();

	ElementType getElementType();
	bool has(PagRevObjParts part);
	void drawAsTriangles(PagShaderProgram *shader, glm::mat4 vp, glm::mat4 v);
	void drawAsPoints(PagShaderProgram *shader, glm::mat4 viewProject);
	void drawAsLines(PagShaderProgram *shader, glm::mat4 viewProject);
	void drawAsDebug(PagShaderProgram *shader, glm::mat4 viewProject);
	void drawAsTextures(PagShaderProgram *shader, glm::mat4 vp, glm::mat4 v, 
					std::vector<GLuint> texture = std::vector<GLuint>());

	bool isValid();
};

