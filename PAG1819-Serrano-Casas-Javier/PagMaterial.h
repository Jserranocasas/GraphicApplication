#pragma once

#include "glm.hpp"
#include "PagShaderProgram.h"

/*
 * @brief   Clase que encapsula las propiedades de un objeto para la iluminacón de dicho objeto
 * @file	PagMaterial.h
 * @author	Javier Serrano Casas
 * @date	14-11-2018
 */
class PagMaterial {
public:
	PagMaterial();
	PagMaterial(const PagMaterial &orig);
	PagMaterial(glm::vec3 kd, glm::vec3 ks, float shi);
	PagMaterial& operator=(const PagMaterial &orig);
	~PagMaterial();

	void setMaterial(glm::vec3 _kd, glm::vec3 _ks, float _shi);
	void sendUniform(PagShaderProgram *shader);
private:
	glm::vec3 kd, ks;	//Constantes difusa y especular
	float shininess;	//Propiedad de brillo para el modelo de Phong
};