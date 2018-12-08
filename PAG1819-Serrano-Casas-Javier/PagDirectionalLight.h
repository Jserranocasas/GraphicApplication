
#pragma once

#include "PagShaderProgram.h"
#include "PagLightApplication.h"

/*
 * @brief	Clase que representa una fuente luminosa direccional
 * @file	PagDirectionalLight.h
 * @author	Javier Serrano Casas
 * @date	20-11-2018
 */
class PagDirectionalLight : public PagLightApplication {
public:
	PagDirectionalLight(glm::vec3 _Id, glm::vec3 _Is,
		glm::vec3 _direction, float _c1, float _c2, float _c3);
	virtual ~PagDirectionalLight();

	virtual void apply(PagShaderProgram& ads, PagCamera camera);
	LightType getType();

private:
	float c1, c2, c3;		// Constantes para la atenuación
	glm::vec3 Id, Is;		// Iluminacion difusa y especular
	glm::vec3 direction;	// Direccion de la fuente luminosa
};