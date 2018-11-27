#pragma once

#include <vector>
#include "PagStructures.h"
#include "PagEnumerations.h"
#include <glm.hpp>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

/*
* @file	  PagVAO.h
* @brief  Clase que representa un VAO que contiene 3 VBOs, uno para
  posiciones y normales, otro para tangentes y otro para textura y 3 
  IBOs, para puntos, lineas, y triangulos.
* @author Javier Serrano Casas
* @date	  01-11-2018
*/
class PagVAO {
public:
	PagVAO();
	~PagVAO();
	std::vector<GLuint> getIndexesWireframe() const;
	std::vector<GLuint> getIndexesTMesh() const;
	std::vector<GLuint> getIndexesCloud() const;
	std::vector<PagPosNorm> getPosNorm() const;
	std::vector<glm::vec2> getTextures() const;
	std::vector<glm::vec3> getTangents() const;
	void addIndexWireFrame(GLuint index);
	void addIndexTMesh(GLuint index);
	void addIndexCloud(GLuint index);
	void addPosNorm(PagPosNorm ppn);
	void addTangent(glm::vec3 tang);
	void addTexture(glm::vec2 txt);
	void drawTriangles(PagRevObjParts part);
	void drawPoints();
	void drawLines();
	void fill();

private:
	GLuint vao;
	GLuint vbo[3] = { 0, 0, 0 };
	GLuint ibo[3] = { 0, 0, 0 };
	std::vector<PagPosNorm> posNorm;		//VBO Vector de posiciones y normales del objeto.
	std::vector<glm::vec2> textures;		//VBO Vector de texturas del objeto.
	std::vector<glm::vec3> tangents;		//VBO Vector de tangentes del objeto.
	std::vector<GLuint> indexesTMesh;		//IBO Topología del objeto para malla de triángulos.
	std::vector<GLuint> indexesCloud;		//IBO Topología del objeto para nube de puntos.
	std::vector<GLuint> indexesWireframe;	//IBO Topología del objeto para modelo alambre.
};