#pragma once

#include "PagVAO.h"
#include "Pag3DElement.h"

/*
 * @file	PagPlane.h
 * @brief	Clase PagPlane que representa objetos 3D planos
 * @author	Javier Serrano Casas
 * @date	26-11-2018
 */
class PagPlane : public Pag3DElement {
public:
	PagPlane(float width, float height, float depth, int tilingH, int tilingV);
	~PagPlane();

	// Métodos para dibujar como nube de puntos, modo de alambre o triangulos
	void drawAsTriangles(PagShaderProgram *shader, glm::mat4 vp, glm::mat4 v);
	void drawAsPoints(PagShaderProgram *shader, glm::mat4 viewProject);
	void drawAsLines(PagShaderProgram *shader, glm::mat4 viewProject);
	void drawAsDebug(PagShaderProgram *shader, glm::mat4 viewProject);

private:
	PagVAO *sides[6];

	// Métodos privados para la inicialización del objeto PagPlane
	void addPointsLevel(float width, float height, float depth, int tilingH, int tilingV, int level);
	void addTexturesCoordenates(float width, float height, float depth, int tilingH, int tilingV);
	void addTangentsCoordenates(float width, float height, float depth, int tilingH, int tilingV);
	void fillVectorVAO();

	void defineMeshTriangles(int tilingH, int tilingV);
	void definePointCloud(int tilingH, int tilingV);
	void defineWireFrame(int tilingH, int tilingV);
	void defineTopology(int tilingH, int tilingV);
};

