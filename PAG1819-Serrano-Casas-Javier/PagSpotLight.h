#pragma once

#include "PagShaderProgram.h"
#include "PagLightApplication.h"

/*
 * @brief	Clase que representa una fuente luminosa de tipo foco
 * @file	PagSpotLight.h
 * @author	Javier Serrano Casas
 * @date	20-11-2018
 */
class PagSpotLight : public PagLightApplication {
public:
	PagSpotLight(glm::vec3 _Id, glm::vec3 _Is, glm::vec3 _direction, glm::vec3 _position, 
				float _exponent, float _angle, float _c1, float _c2, float _c3);
	virtual ~PagSpotLight();

	virtual void apply(PagShaderProgram& ads, PagCamera camera);
	LightType getType();
private:
	glm::vec3 Id, Is;		// Iluminación difusa y especular
	glm::vec3 direction;	// Dirección de la fuente luminosa
	glm::vec3 position;		// Posición de la fuente luminosa
	float exponent, angle;	// Exponente y ángulo del foco
	float c1, c2, c3;		// Constante de atenuación
};