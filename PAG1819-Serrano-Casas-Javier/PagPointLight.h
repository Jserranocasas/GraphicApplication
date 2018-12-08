#pragma once

#include "PagShaderProgram.h"
#include "PagLightApplication.h"

/*
 * @brief	Clase que representa una fuente luminosa puntual
 * @file	PagPointLight.h
 * @author	Javier Serrano Casas
 * @date	20-11-2018
 */
class PagPointLight : public PagLightApplication {
public:
	PagPointLight(glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _position, float _c1, float _c2, float _c3);
	virtual ~PagPointLight();

	virtual void apply(PagShaderProgram& ads, PagCamera camera);
	LightType getType();

private:
	float c1, c2, c3;		// Constantes para la atenuación
	glm::vec3 Id, Is;		// Iluminación difusa y especular
	glm::vec3 position;		// Posición de la fuente luminosa
};